#include"groupmodel.hpp"
#include"db.h"
#include"groupuser.hpp"
#include"user.hpp"
 //创建群组  操作数据库AllGroup表  插入群name和群描述desc
bool GroupModel::createGroup(Group &group)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"insert into AllGroup(groupname,groupdesc) values('%s','%s')",group.getName().c_str(),group.getDesc().c_str());
   //连接数据库并执行操作
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            group.setId(mysql_insert_id(mysql.getConnection()));//从mysql连接中获取主键
            return true;
        }
    }
    return false;
}
//加入群组
void GroupModel::addGroup(int id,int groupid,string role)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"insert into GroupUser values(%d,%d,'%s')",groupid,id,role.c_str());
   //连接数据库并执行操作
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
        
    }
}
//查询用户所在群组信息  返回用户所在群  和群内的全部成员的信息
vector<Group> GroupModel::queryGroups(int userid)
{
    /*
    先根据用户的id查groupuser表该用户有那些组
    然后再查allgroup表返回群组的信息
    */
   //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"select a.id,a.groupname,a.groupdesc from AllGroup a inner join \
                 GroupUser b on a.id=b.groupid where b.userid=%d)",userid);//两个表联查
   //连接数据库并执行操作
    MySQL mysql;
    MYSQL_RES* res;
    vector<Group> groupVec;
    if(mysql.connect())
    {
        res=mysql.query(sql);
        if(res!=nullptr)//查询成功
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);//插入一个相当于一个组
            }
        }
    }
    mysql_free_result(res);
    //再查询group表，看那些人在组里，然后查user表返回信息
    for(Group &group:groupVec)
    {
            //1.组装sql语句
        sprintf(sql,"select a.id,a.name,a.state b.grouprole from User a inner join \
                    GroupUser b on a.id=b.userid where b.groupid=%d)",group.getId());//两个表联查  
        //连接数据库并执行操作
        MYSQL_RES* res;
        if(mysql.connect())
        {
            res=mysql.query(sql);
            if(res!=nullptr)//查询成功
            {
                MYSQL_ROW row;
                while((row=mysql_fetch_row(res))!=nullptr)
                {
                    GroupUser user;
                    user.setId(atoi(row[0]));
                    user.setName(row[1]);
                    user.setState(row[2]);
                    user.setRole(row[3]);
                    group.getUsers().push_back(user);//插入一个相当于一个组
                }
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}


//根据群组id查询群组内组员的id列表，除了userid自己，主要在用户使用群聊业务给其他成员发消息时
vector<int> GroupModel::queryGroupUsers(int userid,int groupid)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"select userid from GroupUser where groupid=%d and userid!=%d",groupid,userid);//两个表联查
   //连接数据库并执行操作
    MySQL mysql;
    MYSQL_RES* res;
    vector<int> idvec;
    if(mysql.connect())
    {
        res=mysql.query(sql);
        if(res!=nullptr)//查询成功
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)
            {
                idvec.push_back(atoi(row[0]));
            }
        }
    }
    mysql_free_result(res);
    return idvec;
}