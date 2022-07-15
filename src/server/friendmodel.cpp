#include"friendmodel.hpp"
#include"user.hpp"
#include"db.h"
#include<vector>


 //��Ӻ��ѵĲ���
bool friendModel::insert(int userid,int friendid)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"insert into Friend value(%d,%d)",userid,friendid);
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


//ɾ�����ѵĲ���
bool friendModel::remove(int userid,int friendid)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"delete from Friend where userid=%d and friendid=%d",userid,friendid);
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


//ͬ���ͻ��˱��غ����б����  ���غ��ѵ�id  name state  
vector<User> friendModel::query(int userid)
{
//1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"select distinct a.id,a.name,a.state from User a inner join Friend b on \
    b.friendid = a.id or b.userid = a.id where b.userid = %d or b.friendid=%d",userid,userid);
   //�������ݿⲢִ�в���
    MySQL mysql;
    MYSQL_RES* res;
    User user;
    vector<User> vec;
    if(mysql.connect())
    {
        MYSQL_ROW row;
        if(res=mysql.query(sql))
        {
            
            while((row=mysql_fetch_row(res))!=nullptr)
            {
                if(atoi(row[0])!=userid)
                {
                    user.setId(atoi(row[0]));
                    user.setName(row[1]);
                    user.setState(row[2]);
                    vec.push_back(user);
                }
            }
        }
    }

    mysql_free_result(res);
    return vec;
}