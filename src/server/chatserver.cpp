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
 * ����һ���ļ������������ģ��Ŀ�������������ҵ��ģ����ô�ģ�����ģ�鶼����Ҫ�޸�
 * ����ģ����ʲô��
 *          1.�������������ɺͿͻ��˵����ӣ�����eventloop�¼�ѭ�����󣺷�װ��epoll������TcpServer���󣺷�װ��socket��أ��̳߳�
 *          2.�����һ����������ѭ�����Ϳ��ԺͿͻ����շ���Ϣ��
 *          3.��������Ϊ���յ���ͬ����Ϣ����ע�᲻ͬ��ҵ����ص������������ͻ������ӺͶϿ��Ļص��������Լ�����ͻ�����Ϣ�Ļص�����
 *          4.ʲô�ǽ����ν����ͨ����������ҵ��ģ�������ģ�黥��Ӱ�죬ҵ��ģ�鲻����ô��չ������ģ�鶼���øģ��ͽн���
 *          5.��ôʵ�֣��ؾ��Ǻ���ָ�룬������ģ�飬�ú���ָ����ú�������ҵ��ģ�飬�ú���ָ��ָ��ͬ�ĺ���������ҵ��ʱ��ֻ��Ҫ���Ӻ���ָ���ָ�����
 *****/




ChatServer::ChatServer(EventLoop* loop,//��Ϊ�����Ա_server�Ĺ�����Ҫ��Щ���������Ը���Ĺ���Ҳ��Ҫ
            const InetAddress& listenAddr,
            const string& nameArg):_server(loop,listenAddr,nameArg),_loop(loop)
{
    //ע�����ӺͶϿ����ӻص�����
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
    
    //ע����Ϣ�ص�����
    _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

    //�����߳�����
    _server.setThreadNum(3);
}
//��������
void ChatServer::start()
{
    _server.start();
}

//�ͻ������ӺͶϿ��ص�����
void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    if(!conn->connected())//���ӳɹ�����true ����ʧ�ܻ����ǿͻ��˶Ͽ�����false
    {
        cout<<"lianjieduankai"<<endl;
        ChatService::instance()->clientCloserException(conn);
        conn->shutdown();//�ͷ���Դ close(clientfd)
    }


}

//�û���д�¼��ص�����
void ChatServer::onMessage(const TcpConnectionPtr& conn,
                        Buffer* buffer,
                        Timestamp time)
{
    string buf=buffer->retrieveAllAsString();//�õ��洢�Ľ��յ�������
    cout<<buf<<endl;
    if(buf.find("msgid")>buf.size())//��Ϣ��ʽ����  û�ҵ�msgid�ֶ�
    {
        json responce;
        responce["msgid"]=NO_MSG;
        responce["errno"]=1;
        conn->send(responce.dump());
        cout<<buf;
    }
    else{
    json js = json::parse(buf);//json����
    //��һ���õ���json�������Ϣ����һ��Ӧ���ǶԲ�ͬ���͵���Ϣ���в�ͬ����Ӧ������ע�ᣬ��½���˳������ݵ�
    //��һ����ͨ��[msgid]����ʶ���ݵ����ͣ����ǲ�Ҫ������ģ����ֱ�ӵ���ҵ��ģ��Ĵ������������ʹﲻ�������Ч��

    //��ȡҵ�������Ķ��󣬰�masgid������    js["msgid"]ʱ��ȡjson������msgid�ֶε�ֵ��������������ʱjson�������͵ģ���Ҫת��int
    auto MsgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());//�ǳ�������ú������ص�ʱ������ĺ���ָ��
    MsgHandler(conn,js,time);//������������    MsgHandler�󶨵����Ǹ������͵����Ǹ�����   ��ȫ���ҵ������ô�ģ������벻�ø�
    }
}
