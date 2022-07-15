#include"groupmodel.hpp"
#include"db.h"
#include"groupuser.hpp"
#include"user.hpp"
 //����Ⱥ��  �������ݿ�AllGroup��  ����Ⱥname��Ⱥ����desc
bool GroupModel::createGroup(Group &group)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"insert into AllGroup(groupname,groupdesc) values('%s','%s')",group.getName().c_str(),group.getDesc().c_str());
   //�������ݿⲢִ�в���
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            group.setId(mysql_insert_id(mysql.getConnection()));//��mysql�����л�ȡ����
            return true;
        }
    }
    return false;
}
//����Ⱥ��
void GroupModel::addGroup(int id,int groupid,string role)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"insert into GroupUser values(%d,%d,'%s')",groupid,id,role.c_str());
   //�������ݿⲢִ�в���
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
        
    }
}
//��ѯ�û�����Ⱥ����Ϣ  �����û�����Ⱥ  ��Ⱥ�ڵ�ȫ����Ա����Ϣ
vector<Group> GroupModel::queryGroups(int userid)
{
    /*
    �ȸ����û���id��groupuser����û�����Щ��
    Ȼ���ٲ�allgroup����Ⱥ�����Ϣ
    */
   //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"select a.id,a.groupname,a.groupdesc from AllGroup a inner join \
                 GroupUser b on a.id=b.groupid where b.userid=%d)",userid);//����������
   //�������ݿⲢִ�в���
    MySQL mysql;
    MYSQL_RES* res;
    vector<Group> groupVec;
    if(mysql.connect())
    {
        res=mysql.query(sql);
        if(res!=nullptr)//��ѯ�ɹ�
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);//����һ���൱��һ����
            }
        }
    }
    mysql_free_result(res);
    //�ٲ�ѯgroup������Щ�������Ȼ���user������Ϣ
    for(Group &group:groupVec)
    {
            //1.��װsql���
        sprintf(sql,"select a.id,a.name,a.state b.grouprole from User a inner join \
                    GroupUser b on a.id=b.userid where b.groupid=%d)",group.getId());//����������  
        //�������ݿⲢִ�в���
        MYSQL_RES* res;
        if(mysql.connect())
        {
            res=mysql.query(sql);
            if(res!=nullptr)//��ѯ�ɹ�
            {
                MYSQL_ROW row;
                while((row=mysql_fetch_row(res))!=nullptr)
                {
                    GroupUser user;
                    user.setId(atoi(row[0]));
                    user.setName(row[1]);
                    user.setState(row[2]);
                    user.setRole(row[3]);
                    group.getUsers().push_back(user);//����һ���൱��һ����
                }
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}


//����Ⱥ��id��ѯȺ������Ա��id�б�����userid�Լ�����Ҫ���û�ʹ��Ⱥ��ҵ���������Ա����Ϣʱ
vector<int> GroupModel::queryGroupUsers(int userid,int groupid)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"select userid from GroupUser where groupid=%d and userid!=%d",groupid,userid);//����������
   //�������ݿⲢִ�в���
    MySQL mysql;
    MYSQL_RES* res;
    vector<int> idvec;
    if(mysql.connect())
    {
        res=mysql.query(sql);
        if(res!=nullptr)//��ѯ�ɹ�
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