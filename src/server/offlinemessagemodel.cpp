#include"offlinemessagemodel.hpp"
#include"db.h"
#include<vector>
#include<json.hpp>


//�洢������Ϣ
bool offlinemsgModel::insert(int userid,string msg)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"insert into OfflineMessage values(%d,'%s')",userid,msg.c_str());
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
//ɾ��������Ϣ
bool offlinemsgModel::remove(int userid)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"delete from OfflineMessage where userid=%d",userid);
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
//��ѯ�Ƿ���������Ϣ
vector<string> offlinemsgModel::query(int userid)
{
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"select message from OfflineMessage where userid = %d",userid);
   //�������ݿⲢִ�в���
    MySQL mysql;
    MYSQL_RES* res;
    vector<string> vec;
    if(mysql.connect())
    {
        if(res=mysql.query(sql))
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)//ѭ����ȡ��Ϣ�����ܲ�ֹһ��������Ϣ
            {
                vec.push_back(row[0]);
            }
            
        }
    }
    mysql_free_result(res);
    //��ѯʧ�� �û������� ���useridΪ-1��name pwd���ǿ�  
    return vec;
}









