#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;


//�ṩ����������Ϣ��ĺ���
class offlinemsgModel
{
public:
    //�洢������Ϣ
    bool insert(int userid,string msg);
    //ɾ��������Ϣ
    bool remove(int userid);
    //��ѯ�Ƿ���������Ϣ
    vector<string> query(int userid);

};