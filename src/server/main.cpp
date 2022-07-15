#include "chatserver.hpp"
#include"chatservice.hpp"
#include<iostream>
#include<signal.h>
#include<stdio.h>
using namespace std;

//为了防止ctrl+c终止服务器
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}


int main(int argc,char* argv[])
{
    if(argc<3)
    {
        perror("command invalid! argc=3!");
        return 0;
    }

    signal(SIGINT,resetHandler);
    EventLoop loop;//epoll
    InetAddress addr(argv[1],atoi(argv[2]));
    ChatServer server(&loop,addr,"ChatServer");
    
    server.start();
    loop.loop();//epoll_wait
    return 0;
}