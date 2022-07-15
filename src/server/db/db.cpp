#include "db.h"
#include <string>
#include <muduo/base/Logging.h>

using namespace std;
using namespace muduo;
//��һ����Ӧ��д�������ļ���
static string server = "127.0.0.1";
static string user = "root";
static string password = "123456";
static string dbname = "chat";

MySQL::MySQL()
{
    _conn = mysql_init(nullptr); //��ʼ�����ݿ����ӷ�����û�д�������
}
MySQL::~MySQL()
{
    if (_conn != nullptr)
        mysql_close(_conn); //�ͷ���Դ
}
//�������ݿ�
bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(), password.c_str(), dbname.c_str(), 3306, nullptr, 0); //���������ӽ���
    if (p != nullptr)
    {
        mysql_query(_conn, "set names gbk"); // c��c++����Ĭ�ϵı����ַ�ʱascii����������ã���MYSQL���������������ʾ��
        LOG_INFO<<"connect mysql success!";
    }
    else 
        LOG_INFO<<"connect mysql fail!";
    return p;
}

//���²���,д���ݿ�
bool MySQL::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "����ʧ��!";
        return false;
    }
    return true;
}

//��ѯ����
MYSQL_RES *MySQL::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "��ѯʧ��!";
        return nullptr;
    }
    return mysql_use_result(_conn); //���ز�ѯ���
}


MYSQL* MySQL::getConnection()
{
    return _conn;
}