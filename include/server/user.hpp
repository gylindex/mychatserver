#pragma once
#include<iostream>
 #include<string>

using namespace std;
//表映射类  不设计cpp文件了

class User
{
public:
    User(int id=-1,string name="",string pwd="",string state="offline"):_id(id),_name(name),_password(pwd),_state(state){}

    //对外接口
    void setId(int id){this->_id=id;}
    void setName(string name){this->_name=name;}
    void setPassword(string pwd){this->_password=pwd;}
    void setState(string state){this->_state=state;}

    int getId(){return this->_id;}
    string getName(){return this->_name;}
    string getPassword(){return this->_password;}
    string getState(){return this->_state;}
private:
    int _id;
    string _name;
    string _password;
    string _state;
};