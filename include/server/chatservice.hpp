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
// c++11�±�׼���൱��define����ϸ����ҵ��ص��������ͣ��������ָ��������ģ���ҵ��ģ�������
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

//ҵ������ ���Ϊ����ģʽ  ���캯��˽�л�  ��̬����  ����ӿڻ�ȡ��������
class ChatService
{
public:
    //��ȡ��������Ľӿں���
    static ChatService *instance();
    //�����½ҵ��
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //����ע��ҵ��
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //����һ��һ����ҵ��
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //��Ӻ��ѵĴ�����
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //ɾ�����ѵĴ�����
    void removeFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //����Ⱥ��Ĵ�����
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //����Ⱥ�Ĵ�����
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //Ⱥ���촦����
    void chatGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //����ע��ҵ��Ļص�����  ɾ����������map��  �޸����ݿ�����״̬  �������
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //��������Ϣ�ϱ���redis��ת���������������ĺ���
    void handleRedisSubscribeMessage(const int userid, const string msg);

    //��ȡ��Ϣ��Ӧ�Ĵ�����   ������������������Ǹ�ҵ��  ��½  ע��  ��Ϣ
    MsgHandler getHandler(int msgid);

    //����ͻ����쳣�˳��Ĵ�����
    void clientCloserException(const TcpConnectionPtr& conn);

    //�������쳣�˳�ʱ�����÷���״̬�ĺ���
    void reset();

private:
    //���캯��˽�л�
    ChatService();

    //�洢��Ϣid�Ͷ�Ӧ��ҵ������   <int,void*>
    unordered_map<int, MsgHandler> _msgHanderMap;

    //�������ݿ�user��Ķ���
    UserModel _userModel;
    //�������ݿ�offlinemessage��Ķ���
    offlinemsgModel _offlineMsgmodel;
    //����Friend��Ķ���
    friendModel _friendModel;
    //����Ⱥ��Ķ���
    GroupModel _groupModel;


    //�洢�����û�������  ������Ҫ�������û�����Ϣ��Ҫ��Щ����
    unordered_map<int,TcpConnectionPtr> _userConnMap;

    //���廥��������֤_userConnMap���̰߳�ȫ
    mutex _connMutex;

    Redis _redis;

};