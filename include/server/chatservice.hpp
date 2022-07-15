#pragma once

#include <iostream>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <unordered_map>
#include<mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;
#include "json.hpp"
using json = nlohmann::json;
#include "public.hpp"
#include"usermodel.hpp"
#include"offlinemessagemodel.hpp"
#include"friendmodel.hpp"
#include"groupmodel.hpp"
#include"redis.hpp"
// c++11新标准，相当于define，详细处理业务回调函数类型，这个函数指针是网络模块和业务模块的桥梁
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

//业务处理类 设计为单例模式  构造函数私有化  静态对象  对外接口获取单例对象
class ChatService
{
public:
    //获取单例对像的接口函数
    static ChatService *instance();
    //处理登陆业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //处理一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //添加好友的处理函数
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //删除好友的处理函数
    void removeFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //创建群组的处理函数
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //加入群聊处理函数
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //群聊天处理函数
    void chatGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注销业务的回调函数  删除本地在线map表  修改数据库在线状态  解除订阅
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //服务器消息上报到redis，转发到其他服务器的函数
    void handleRedisSubscribeMessage(const int userid, const string msg);

    //获取消息对应的处理器   用这个函数决定调用那个业务  登陆  注册  消息
    MsgHandler getHandler(int msgid);

    //处理客户端异常退出的处理器
    void clientCloserException(const TcpConnectionPtr& conn);

    //服务器异常退出时，重置服务状态的函数
    void reset();

private:
    //构造函数私有化
    ChatService();

    //存储消息id和对应的业务处理函数   <int,void*>
    unordered_map<int, MsgHandler> _msgHanderMap;

    //操作数据库user表的对象
    UserModel _userModel;
    //操作数据库offlinemessage表的对象
    offlinemsgModel _offlineMsgmodel;
    //操作Friend表的对象
    friendModel _friendModel;
    //操作群组的对象
    GroupModel _groupModel;


    //存储在线用户的连接  服务器要主动给用户发消息需要这些连接
    unordered_map<int,TcpConnectionPtr> _userConnMap;

    //定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    Redis _redis;

};