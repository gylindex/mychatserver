#pragma once

#include<iostream>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
/*
1.组合TcpServer对象
2.创建EentLoop事件循环对象指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器类的构造函数中，注册处理连接/断开连接和处理读写事件的回调函数
5.设置服务器端的线程池线程数量，一般和核数相关

*/
//网络连接类
class ChatServer
{
public:
    ChatServer(EventLoop* loop,//因为该类成员_server的构造需要这些参数，所以该类的构造也需要
            const InetAddress& listenAddr,
            const string& nameArg);
    //启动服务
    void start();
    
private:
    //客户端连接和断开回调函数
    void onConnection(const TcpConnectionPtr& conn);
    //用户读写事件回调函数
    void onMessage(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Timestamp time);
    TcpServer _server;//实现了listensocket，线程池等功能的类对象
    EventLoop* _loop;//实现了epoll等功能的类对象指针
};