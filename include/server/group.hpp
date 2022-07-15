#pragma once
#include <string>
#include<vector>
#include"groupuser.hpp"
using namespace std;

class Group
{
public:
    Group(int id = -1, string name = "", string desc = "") : _id(id), _name(name), _desc(desc) {}

    void setId(int id){this->_id=id;};
    void setName(string name){this->_name=name;};
    void setDesc(string desc){this->_desc=desc;};


    int getId(){return this->_id;}
    string getName(){return this->_name;}
    string getDesc(){return this->_desc;}
    vector<GroupUser> &getUsers(){return this->_Uservec;}

private:
    int _id;
    string _name;
    string _desc;
    vector<GroupUser> _Uservec;
};