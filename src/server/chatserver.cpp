#include "chatserver.hpp"
#include <functional>
#include "json.hpp"
#include<string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;
#include "chatservice.hpp"
#include<muduo/base/Logging.h>

using namespace muduo;
/****
 * 就这一个文件就完成了网络模块的开发，而且无论业务模块怎么改，网络模块都不需要修改
 * 网络模块做什么：
 *          1.首先最基本的完成和客户端的连接，创建eventloop事件循环对象：封装了epoll，创建TcpServer对象：封装了socket相关，线程池
 *          2.做完第一步，再启动循环，就可以和客户端收发消息了
 *          3.接下来就为接收到不同的消息类型注册不同的业务处理回调函数，包括客户端连接和断开的回调函数，以及处理客户端消息的回调函数
 *          4.什么是解耦？所谓解耦通俗来讲就是业务模块和网络模块互不影响，业务模块不管怎么发展，网络模块都不用改，就叫解耦
 *          5.怎么实现？秘诀是函数指针，在网络模块，用函数指针调用函数，在业务模块，让函数指针指向不同的函数，更改业务时，只需要增加函数指针的指向就行
 *****/




ChatServer::ChatServer(EventLoop* loop,//因为该类成员_server的构造需要这些参数，所以该类的构造也需要
            const InetAddress& listenAddr,
            const string& nameArg):_server(loop,listenAddr,nameArg),_loop(loop)
{
    //注册连接和断开连接回调函数
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
    
    //注册消息回调函数
    _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

    //设置线程数量
    _server.setThreadNum(3);
}
//启动服务
void ChatServer::start()
{
    _server.start();
}

//客户端连接和断开回调函数
void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    if(!conn->connected())//连接成功返回true 连接失败或者是客户端断开返回false
    {
        cout<<"lianjieduankai"<<endl;
        ChatService::instance()->clientCloserException(conn);
        conn->shutdown();//释放资源 close(clientfd)
    }


}

//用户读写事件回调函数
void ChatServer::onMessage(const TcpConnectionPtr& conn,
                        Buffer* buffer,
                        Timestamp time)
{
    string buf=buffer->retrieveAllAsString();//得到存储的接收到的数据
    cout<<buf<<endl;
    if(buf.find("msgid")>buf.size())//消息格式错误  没找到msgid字段
    {
        json responce;
        responce["msgid"]=NO_MSG;
        responce["errno"]=1;
        conn->send(responce.dump());
        cout<<buf;
    }
    else{
    json js = json::parse(buf);//json编码
    //上一步得到了json解码的消息，下一步应该是对不同类型的消息进行不同的响应，包括注册，登陆，退出，数据等
    //下一步就通过[msgid]来标识数据的类型，但是不要在网络模块里直接调用业务模块的处理函数，那样就达不到解耦的效果

    //获取业务处理器的对象，把masgid传给他    js["msgid"]时获取json对象内msgid字段的值，但是他的类型时json内置类型的，需要转成int
    auto MsgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());//非常的巧妙，该函数返回的时事务处理的函数指针
    MsgHandler(conn,js,time);//调用事务处理函数    MsgHandler绑定的是那个函数就调用那个函数   完全解耦，业务再怎么改，这块代码不用改
    }
}
