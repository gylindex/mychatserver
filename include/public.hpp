#pragma once


/***
 * server��client������ͷ�ļ�
 * */


enum EnMsgType
{
    LOGIN_MSG=1,        //��½��Ϣ
    LOGIN__MSG_ACK,     //��½��Ӧ��Ϣ
    LOGINOUT_MSG,       //�˳���½
    REG_MSG,            //ע����Ϣ
    REG_MSG_ACK,         //ע����Ӧ��Ϣ
    ONE_CHAT_MSG,       //������Ϣ
    ADD_FRIEND_MSG,      //��Ӻ��ѵ���Ϣ 

    CREATE_GROUP_MSG,   //����Ⱥ��
    ADD_GROUP_MSG,      //���Ⱥ��
    GROUP_CHAT_MSG,      //Ⱥ����

    ADD_FRIEND_MSG_ACK,  //��Ӻ�����Ӧ��Ϣ
    OFFLOGIN_MSG_ACK,   //�˳���½��Ӧ
    NO_MSG,             //��Ϣ��ʽ������Ӧ
    RM_FRIEND_MSG      //ɾ������
    
};