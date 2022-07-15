#pragma once

#include <mysql/mysql.h>
#include <string>
using namespace std;

class MySQL
{
public:
    MySQL();

    ~MySQL();

    //�������ݿ�
    bool connect();

    //���²���
    bool update(string sql);

    //��ѯ����
    MYSQL_RES *query(string sql);

    //��ȡ���ӷ�
    MYSQL* getConnection();
private:
    MYSQL *_conn;
};