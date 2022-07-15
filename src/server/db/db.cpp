#include "db.h"
#include <string>
#include <muduo/base/Logging.h>

using namespace std;
using namespace muduo;
//这一部分应该写在配置文件中
static string server = "127.0.0.1";
static string user = "root";
static string password = "123456";
static string dbname = "chat";

MySQL::MySQL()
{
    _conn = mysql_init(nullptr); //初始化数据库连接符，并没有创建连接
}
MySQL::~MySQL()
{
    if (_conn != nullptr)
        mysql_close(_conn); //释放资源
}
//连接数据库
bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(), password.c_str(), dbname.c_str(), 3306, nullptr, 0); //真正的连接建立
    if (p != nullptr)
    {
        mysql_query(_conn, "set names gbk"); // c和c++代码默认的编码字符时ascii，如果不设置，从MYSQL上拉下里的中文显示？
        LOG_INFO<<"connect mysql success!";
    }
    else 
        LOG_INFO<<"connect mysql fail!";
    return p;
}

//更新操作,写数据库
bool MySQL::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "更新失败!";
        return false;
    }
    return true;
}

//查询操作
MYSQL_RES *MySQL::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "查询失败!";
        return nullptr;
    }
    return mysql_use_result(_conn); //返回查询结果
}


MYSQL* MySQL::getConnection()
{
    return _conn;
}