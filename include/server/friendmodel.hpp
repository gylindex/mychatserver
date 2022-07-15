#pragma once
#include<string>
#include<vector>
#include"user.hpp"
#include"db.h"
using namespace std;

//提供操作friend表的函数 完成添加好友的操作

class friendModel
{
public:
    //添加好友的操作
    bool insert(int userid,int friendid);
    //删除好友的操作
    bool remove(int userid,int friendid);
    //同步客户端本地好友列表操作
    vector<User> query(int userid);
    

};