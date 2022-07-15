#pragma once
#include"group.hpp"
#include<string>
using namespace std;

class GroupModel
{
public:
    //����Ⱥ��
    bool createGroup(Group &group);
    //����Ⱥ��
    void addGroup(int id,int groupid,string role);
    //��ѯ�û�����Ⱥ����Ϣ
    vector<Group> queryGroups(int userid);
    //����Ⱥ��id��ѯȺ������Ա��id�б�����userid�Լ�����Ҫ���û�ʹ��Ⱥ��ҵ���������Ա����Ϣʱ
    vector<int> queryGroupUsers(int userid,int groupid);

};