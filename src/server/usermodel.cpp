#include"usermodel.hpp"
#include<iostream>

#include<db.h>
using namespace std;

//user���������ݺ���
bool UserModel::insert(User &user)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"insert into User(name,password,state) values('%s','%s','%s')",
            user.getName().c_str(),user.getPassword().c_str(),user.getState().c_str());
   //�������ݿⲢִ�в���
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            //��ȡ�û�id�����ý�user������
            int id= mysql_insert_id(mysql.getConnection());
            user.setId(id);
            return true;
        }
    }
    return false;
}

User UserModel::query(int id)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"select * from User where id = %d",id);
   //�������ݿⲢִ�в���
    MySQL mysql;
    MYSQL_RES* res;
    User user;
    if(mysql.connect())
    {
        if(res=mysql.query(sql))
        {
            MYSQL_ROW row=mysql_fetch_row(res);
            if(row)
            {
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }
    mysql_free_result(res);
    //��ѯʧ�� �û������� ���useridΪ-1��name pwd���ǿ�  
    return user;
}

bool UserModel::updatestate(User user)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"update User set state='%s' where id=%d",user.getState().c_str(),user.getId());
   //�������ݿⲢִ�в���
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
    }
    //����ʧ��
    return false;
}


//����user���state��
bool UserModel::resetstate()
{
    //1.��װsql���
    char sql[1024]="update User set state='offline' where state='online'";
   //�������ݿⲢִ�в���
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}