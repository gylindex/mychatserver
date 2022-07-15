#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;


//提供操作离线消息表的函数
class offlinemsgModel
{
public:
    //存储离线消息
    bool insert(int userid,string msg);
    //删除离线消息
    bool remove(int userid);
    //查询是否有离线消息
    vector<string> query(int userid);

};