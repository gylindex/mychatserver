#include"usermodel.hpp"
#include<iostream>

#include<db.h>
using namespace std;

//user表增加数据函数
bool UserModel::insert(User &user)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"insert into User(name,password,state) values('%s','%s','%s')",
            user.getName().c_str(),user.getPassword().c_str(),user.getState().c_str());
   //连接数据库并执行操作
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            //获取用户id并设置进user对象中
            int id= mysql_insert_id(mysql.getConnection());
            user.setId(id);
            return true;
        }
    }
    return false;
}

User UserModel::query(int id)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"select * from User where id = %d",id);
   //连接数据库并执行操作
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
    //查询失败 用户不存在 这个userid为-1，name pwd都是空  
    return user;
}

bool UserModel::updatestate(User user)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"update User set state='%s' where id=%d",user.getState().c_str(),user.getId());
   //连接数据库并执行操作
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
    }
    //更新失败
    return false;
}


//重置user表的state项
bool UserModel::resetstate()
{
    //1.组装sql语句
    char sql[1024]="update User set state='offline' where state='online'";
   //连接数据库并执行操作
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