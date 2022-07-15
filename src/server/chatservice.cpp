#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <user.hpp>
#include <usermodel.hpp>
#include <string>
#include <mutex>
using namespace std;
using namespace muduo;

//获取单例对像的接口函数
ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

//构造函数   注册消息以及对应的回调
ChatService::ChatService()
{
    // map中存储消息类型以及消息类型对应的处理函数的函数指针，这是c++11的新写法，有点让人看不懂
    //这个map定义为<int,function>，这个function时c++11里的，他其实就相当于一个函数指针，但是又比函数指针功能多，他能规定函数的返回值和参数列表
    _msgHanderMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    //用户注册功能注册
    _msgHanderMap.insert(pair<int, MsgHandler>(REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3))); //不要看不懂了，两种写法，pair和{}都行
    //用户一对一聊天注册
    _msgHanderMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    //添加好友功能注册
    _msgHanderMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    //删除好友功能注册
    _msgHanderMap.insert({RM_FRIEND_MSG, std::bind(&ChatService::removeFriend, this, _1, _2, _3)});
    //创建群组的处理函数
    _msgHanderMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    //加入群聊处理函数
    _msgHanderMap.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    //群聊天处理函数
    _msgHanderMap.insert({GROUP_CHAT_MSG, std::bind(&ChatService::chatGroup, this, _1, _2, _3)});
    //注册退出登陆业务
    _msgHanderMap.insert({LOGINOUT_MSG, std::bind(&ChatService::loginout, this, _1, _2, _3)});
    //连接redis服务器
    if(_redis.connect())
    {
        //设置消息上报的回调函数
        _redis.init_notify_handler(std::bind(&ChatService::handleRedisSubscribeMessage,this,_1,_2));
    }
}

//获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgid) //这个参数是客户端传过来的，从中解析出来直接调用这个函数就获取了对应的处理函数的函数指针
{
    if (_msgHanderMap.find(msgid) == _msgHanderMap.end()) //防止出错，如果时为定义msgid，那么就会添加一个消息类型到map，这是我不希望的
    {
        //不能这么做，这个会杀死进程。。。
        // LOG_ERROR <<" msgid: "<<msgid<<"not find handler!";
        //返回一个默认操作  空操作
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time)
        {
            LOG_ERROR << " msgid: " << msgid << " not find handler!";
        };
    }
    else
        return _msgHanderMap[msgid];
}





//处理登陆业务  id  pwd   先判断有没有这个id号存在  再判断密码是否正确   再判断登陆状态
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"];
    string pwd = js["password"];
    User user = _userModel.query(id);
    if (user.getId() == id && user.getPassword() == pwd)
    {
        if (user.getState() == "online")
        {
            //登陆失败  账号已经登陆
            json responce;
            responce["msgid"] = LOGIN__MSG_ACK;
            responce["errno"] = 2;
            responce["errmsg"] = "该账号已经在线，登陆失败";
            conn->send(responce.dump());
        }
        else
        {
            //登陆成功
            {
                lock_guard<mutex> lock(_connMutex); // c++11定义的自动释放的互斥锁
                _userConnMap.insert({id, conn});
            }

            //id用户；登陆成功后，向redis订阅该通道号
            _redis.subscribe(id);

            user.setState("online");
            bool update = _userModel.updatestate(user); //更新登陆状态

            json responce;
            responce["msgid"] = LOGIN__MSG_ACK;
            responce["errno"] = 0;
            responce["id"] = id;
            responce["name"] = user.getName();
            responce["state"] = user.getState();
            //查询该用户是否有离线消息 有的话，查询数据库得到该消息并删除离线消息
            vector<string> vec = _offlineMsgmodel.query(id);
            if (!vec.empty())
            {
                //有离线消息
                responce["offlinemsg"] = vec;
                //查询完了之后删除该用户的所有离线消息
                _offlineMsgmodel.remove(id);
            }
            
            //检查该用户是否有好友
            vector<User> vecfriend=_friendModel.query(id);
            if(!vecfriend.empty())
            {
                vector<string> vec2;
                //不为空返回好友信息
                for(User &user:vecfriend)
                {
                    json js;
                    js["id"]=user.getId();
                    js["name"]=user.getName();
                    js["state"]=user.getState();
                    vec2.push_back(js.dump());
                }
                responce["friends"]=vec2;

            }


            //更新登陆状态失败
            if (!update)
            {
                responce["errno"] = 3;
                responce["errmsg"] = "登陆状态更新失败！";
            }

            conn->send(responce.dump());
        }
    }
    else
    {
        //登陆失败
        json responce;
        responce["msgid"] = LOGIN__MSG_ACK;
        responce["errno"] = 1;
        responce["errmsg"] = "账号不存在或密码错误";
        conn->send(responce.dump());
    }
}

//处理注销业务的回调函数  删除本地在线map表  修改数据库在线状态  解除订阅
void ChatService::loginout(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid=js["id"].get<int>();
    {
        //删除本地连接map表里的在线信息
        lock_guard<mutex> lock(_connMutex);
        auto it=_userConnMap.find(userid);
        if(it!=_userConnMap.end())
        {
            _userConnMap.erase(it);
        }
    }
    //解除订阅
    _redis.unsubscribe(userid);

    //更新用户数据库在线信息
    User user(userid,"","","offline");
    _userModel.updatestate(user);
}

//处理注册业务   
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];
    User user;
    user.setName(name);
    user.setPassword(pwd);
    bool state = _userModel.insert(user); //插入数据库User表 插入成功返回true
    if (state)
    {
        //注册成功   发送登陆响应类型的消息
        json responce;
        responce["msgid"] = REG_MSG_ACK;
        responce["errno"] = 0;
        responce["id"] = user.getId();
        conn->send(responce.dump());
    }
    else
    {
        //注册失败
        json responce;
        responce["msgid"] = REG_MSG_ACK;
        responce["errno"] = 1;
        conn->send(responce.dump());
    }
}

//一对一聊天业务处理函数，收到消息后间车msgid表示聊天信息时为线程调用此函数
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    //获取toid
    int toid = js["toid"].get<int>();
    //查看本地在线信息表看对方是否在线
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end())//在线
        {
            //在线  转发消息 必须再锁内进行，因为可能发送过程中对方已经离线了
            it->second->send(js.dump()); //直接转发就行  接收方解析其中的msg字段就行 很方便
            return;
        }
    }

    //如果不在线，查询数据库是否在线  如果数据库也不在线，则真的不在线，如果数据库在线则是登录到其他服务器上了
    User user=_userModel.query(toid);
    if(user.getState()=="online")//在其他主机上登陆了
    {
        //publish发布
        _redis.publish(toid,js.dump());
        return;
    }

    //不在线 存储离线消息
    _offlineMsgmodel.insert(toid, js.dump());
}


//添加好友的处理函数
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid=js["id"].get<int>();
    int friendId=js["friendid"].get<int>();
    _friendModel.insert(userid,friendId);//添加好友信息
    //添加之后再返回给用户好友列表
    User user=_userModel.query(friendId);//只查询这个好友的信息
    json response;
    response["msgid"]=ADD_FRIEND_MSG_ACK;
    response["friend"]={{"id",user.getId()},{"name",user.getName()},{"state",user.getState()}};
    conn->send(response.dump());
}

//删除好友的处理函数
void ChatService::removeFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id=js["id"];
    int friendid=js["friendid"];
    bool res=_friendModel.remove(id,friendid);
    json jso;
    if(res)
        jso["rmfriend"]="true";
    else
        jso["rmfriend"]="false";
    conn->send(jso.dump());//删除成功则返回true给客户端，客户端更新本地好友列表
}

//创建群组的处理函数  userid  groupname  desc
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid=js["id"].get<int>();
    string groupname=js["groupname"];
    string desc=js["groupdesc"];
    Group group(-1,groupname,desc);//创建一个群对象
    //创建群
    if(_groupModel.createGroup(group))//群如果存在就会创建失败
    {
        //创建成功  把创建人存入该群聊
        _groupModel.addGroup(userid,group.getId(),"creator");
    }


}
//加入群聊处理函数  groupid  userid
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid=js["id"];
    int groupid=js["groupid"];
    _groupModel.addGroup(userid,groupid,"normal");

}
//群聊天处理函数   groupid userid    msg
void ChatService::chatGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int groupid=js["groupid"].get<int>();
    int userid=js["id"].get<int>();

    vector<int> vec = _groupModel.queryGroupUsers(userid,groupid);//返回群聊内其他成员
    cout<<vec.size()<<endl;
    
    User user;
    for(auto itv=vec.begin();itv!=vec.end();itv++)
    {
        int id=*itv;
        cout<<id<<endl;
        lock_guard<mutex> lock(_connMutex);//加锁，不让他们下线
        auto it=_userConnMap.find(id);
        user=_userModel.query(id);
        if(it!=_userConnMap.end())
        {
            //在线
            it->second->send(js.dump());//原封不动的转发
        }
        else if(user.getState() == "online")
        {
            //在其他服务器上在线  发布
            _redis.publish(id,js.dump());
        }
        else
        {
            //不在线
            _offlineMsgmodel.insert(it->first,js.dump());

        }
    }
}

//收到来自redis消息队列的消息时，转发给用户的函数   用户在线直接发给用户  不在线发到离线消息表中
void ChatService::handleRedisSubscribeMessage(const int userid, const string msg)
{
    //加锁查本地在线map表
    lock_guard<mutex> lock(_connMutex);
    auto it=_userConnMap.find(userid);
    if(it!=_userConnMap.end())
    {
        //在线  发送msg到用户
        it->second->send(msg);
        return;
    }
    //不在线 存储离线消息
    _offlineMsgmodel.insert(userid,msg);
}






//处理客户端异常退出的处理器   做两件事：删除在线用户连接map表中的对应字段  修改数据库登陆状态
void ChatService::clientCloserException(const TcpConnectionPtr &conn)
{
    User user;
    {
        //线程临界资源
        lock_guard<mutex> lock(_connMutex);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); it++)
        {
            if (it->second == conn) //找到了对应的连接
            {

                user.setId(it->first);
                //删除map表中的连接信息
                _userConnMap.erase(it);
                break;
            }
        }
    }
    //修改数据库登陆状态
    if (user.getId() != -1)
    {
        user.setState("offline");
        _userModel.updatestate(user);
    }

    //取消订阅通道
    _redis.unsubscribe(user.getId());
}


//服务器ctrl+c异常退出时，重置服务状态的函数
 void ChatService::reset()
 {
    //把onlin设置为offline
    _userModel.resetstate();
    //取消订阅所有通道
    //服务器推出后，redis客户端终止，自动退订所有通道
 }