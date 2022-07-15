#pragma once
#include"group.hpp"
#include<string>
using namespace std;

class GroupModel
{
public:
    //创建群组
    bool createGroup(Group &group);
    //加入群组
    void addGroup(int id,int groupid,string role);
    //查询用户所在群组信息
    vector<Group> queryGroups(int userid);
    //根据群组id查询群组内组员的id列表，除了userid自己，主要在用户使用群聊业务给其他成员发消息时
    vector<int> queryGroupUsers(int userid,int groupid);

};