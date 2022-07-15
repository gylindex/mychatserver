#pragma once


/***
 * server和client公共的头文件
 * */


enum EnMsgType
{
    LOGIN_MSG=1,        //登陆消息
    LOGIN__MSG_ACK,     //登陆响应消息
    LOGINOUT_MSG,       //退出登陆
    REG_MSG,            //注册消息
    REG_MSG_ACK,         //注册响应消息
    ONE_CHAT_MSG,       //聊天消息
    ADD_FRIEND_MSG,      //添加好友的消息 

    CREATE_GROUP_MSG,   //创建群组
    ADD_GROUP_MSG,      //添加群组
    GROUP_CHAT_MSG,      //群聊天

    ADD_FRIEND_MSG_ACK,  //添加好友响应消息
    OFFLOGIN_MSG_ACK,   //退出登陆响应
    NO_MSG,             //消息格式错误响应
    RM_FRIEND_MSG      //删除好友
    
};