#include"offlinemessagemodel.hpp"
#include"db.h"
#include<vector>
#include<json.hpp>


//存储离线消息
bool offlinemsgModel::insert(int userid,string msg)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"insert into OfflineMessage values(%d,'%s')",userid,msg.c_str());
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
//删除离线消息
bool offlinemsgModel::remove(int userid)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"delete from OfflineMessage where userid=%d",userid);
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
//查询是否有离线消息
vector<string> offlinemsgModel::query(int userid)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"select message from OfflineMessage where userid = %d",userid);
   //连接数据库并执行操作
    MySQL mysql;
    MYSQL_RES* res;
    vector<string> vec;
    if(mysql.connect())
    {
        if(res=mysql.query(sql))
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)//循环获取消息，可能不止一条离线消息
            {
                vec.push_back(row[0]);
            }
            
        }
    }
    mysql_free_result(res);
    //查询失败 用户不存在 这个userid为-1，name pwd都是空  
    return vec;
}









