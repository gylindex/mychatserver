#pragma once

#include<iostream>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
/*
1.���TcpServer����
2.����EentLoop�¼�ѭ������ָ��
3.��ȷTcpServer���캯����Ҫʲô���������ChatServer�Ĺ��캯��
4.�ڵ�ǰ��������Ĺ��캯���У�ע�ᴦ������/�Ͽ����Ӻʹ����д�¼��Ļص�����
5.���÷������˵��̳߳��߳�������һ��ͺ������

*/
//����������
class ChatServer
{
public:
    ChatServer(EventLoop* loop,//��Ϊ�����Ա_server�Ĺ�����Ҫ��Щ���������Ը���Ĺ���Ҳ��Ҫ
            const InetAddress& listenAddr,
            const string& nameArg);
    //��������
    void start();
    
private:
    //�ͻ������ӺͶϿ��ص�����
    void onConnection(const TcpConnectionPtr& conn);
    //�û���д�¼��ص�����
    void onMessage(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Timestamp time);
    TcpServer _server;//ʵ����listensocket���̳߳صȹ��ܵ������
    EventLoop* _loop;//ʵ����epoll�ȹ��ܵ������ָ��
};