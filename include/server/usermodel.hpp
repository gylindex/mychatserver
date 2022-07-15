#pragma once

#include "user.hpp"
#include "db.h"

// user表的数据操作类
class UserModel
{
public:
    //user表增加数据函数
    bool insert(User &user);
    //根据id查询账户信息
    User query(int id);
    //更新账户信息
    bool updatestate(User user);

    //重置user表的state项
    bool resetstate();
};