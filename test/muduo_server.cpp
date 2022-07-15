/*************
muduo网路库给用户提供了两个主要的类
TcpServer：用于编写服务器程序
TcpClient：用于编写客户端程序

封装了：epoll+线程池
好处：能够把网络IO的代码和业务代码分开，我们只需要关系用户的断开和连接，用户的可读事件
***************/

#include<muduo/net/TcpServer.h>
#include<iostream>
#include<muduo/net/EventLoop.h>
#include<functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*
1.组合TcpServer对象
2.创建EentLoop事件循环对象指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器类的构造函数中，注册处理连接/断开连接和处理读写事件的回调函数
5.设置服务器端的线程池线程数量，一般和核数相关

*/

class ChatServer
{
private:
    TcpServer _server;
    EventLoop* _loop;


    //我们的主要关注onConnection和onMessage这两个函数
    //用户连接回调函数
    void onConnection(const TcpConnectionPtr& conn)
    {
        if(conn->connected())//返回连接状态，true或者false
        {   //打印连接信息  ip+port
            cout<<conn->peerAddress().toIpPort()<< " -> "<<conn->localAddress().toIpPort()<<" state:online "<<endl;
        }
        else
        {
            conn->shutdown();//close(clientfd)
            //_loop->quit();//quit epoll
            cout<<conn->peerAddress().toIpPort()<< " -> "<<conn->localAddress().toIpPort()<<" state:offline "<<endl;
        }
    }
    //用户读写回调函数
    void onMessage(const TcpConnectionPtr& conn,//连接
                            Buffer* buffer,//缓冲区
                            Timestamp time)//时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout<<" recv data: "<<buf<<" time: "<<time.toString()<<endl;
        conn->send(buf);
    }

public:
    ChatServer(EventLoop* loop,const InetAddress& listenAddr,const string& nameArg):_server(loop,listenAddr,nameArg),_loop(loop)
            {
                //给服务器注册用户连接的创建和断开回调，TCPserver里的setConnectionCallback
                //该函数有参数const ConnectionCallback& cb，这个参数就是回调的函数，用setConnectionCallback来设置他
                _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));

                //给服务器注册用户读写事件回调，setMessageCallback
                _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

                //设置线程数量
                _server.setThreadNum(3);
            }
    //开启事件循环
    void start()
    {
        _server.start();
    }
};

int main()
{
    EventLoop loop;//epoll
    InetAddress addr("127.0.0.1",8888);//设置listensocket
    ChatServer server(&loop,addr,"charserver");
    server.start();//
    loop.loop();//epoll_wait
}

