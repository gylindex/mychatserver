/*************
muduo��·����û��ṩ��������Ҫ����
TcpServer�����ڱ�д����������
TcpClient�����ڱ�д�ͻ��˳���

��װ�ˣ�epoll+�̳߳�
�ô����ܹ�������IO�Ĵ����ҵ�����ֿ�������ֻ��Ҫ��ϵ�û��ĶϿ������ӣ��û��Ŀɶ��¼�
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
1.���TcpServer����
2.����EentLoop�¼�ѭ������ָ��
3.��ȷTcpServer���캯����Ҫʲô���������ChatServer�Ĺ��캯��
4.�ڵ�ǰ��������Ĺ��캯���У�ע�ᴦ������/�Ͽ����Ӻʹ����д�¼��Ļص�����
5.���÷������˵��̳߳��߳�������һ��ͺ������

*/

class ChatServer
{
private:
    TcpServer _server;
    EventLoop* _loop;


    //���ǵ���Ҫ��עonConnection��onMessage����������
    //�û����ӻص�����
    void onConnection(const TcpConnectionPtr& conn)
    {
        if(conn->connected())//��������״̬��true����false
        {   //��ӡ������Ϣ  ip+port
            cout<<conn->peerAddress().toIpPort()<< " -> "<<conn->localAddress().toIpPort()<<" state:online "<<endl;
        }
        else
        {
            conn->shutdown();//close(clientfd)
            //_loop->quit();//quit epoll
            cout<<conn->peerAddress().toIpPort()<< " -> "<<conn->localAddress().toIpPort()<<" state:offline "<<endl;
        }
    }
    //�û���д�ص�����
    void onMessage(const TcpConnectionPtr& conn,//����
                            Buffer* buffer,//������
                            Timestamp time)//ʱ����Ϣ
    {
        string buf = buffer->retrieveAllAsString();
        cout<<" recv data: "<<buf<<" time: "<<time.toString()<<endl;
        conn->send(buf);
    }

public:
    ChatServer(EventLoop* loop,const InetAddress& listenAddr,const string& nameArg):_server(loop,listenAddr,nameArg),_loop(loop)
            {
                //��������ע���û����ӵĴ����ͶϿ��ص���TCPserver���setConnectionCallback
                //�ú����в���const ConnectionCallback& cb������������ǻص��ĺ�������setConnectionCallback��������
                _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));

                //��������ע���û���д�¼��ص���setMessageCallback
                _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

                //�����߳�����
                _server.setThreadNum(3);
            }
    //�����¼�ѭ��
    void start()
    {
        _server.start();
    }
};

int main()
{
    EventLoop loop;//epoll
    InetAddress addr("127.0.0.1",8888);//����listensocket
    ChatServer server(&loop,addr,"charserver");
    server.start();//
    loop.loop();//epoll_wait
}

