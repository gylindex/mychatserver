#pragma once

#include "user.hpp"
#include "db.h"

// user������ݲ�����
class UserModel
{
public:
    //user���������ݺ���
    bool insert(User &user);
    //����id��ѯ�˻���Ϣ
    User query(int id);
    //�����˻���Ϣ
    bool updatestate(User user);

    //����user���state��
    bool resetstate();
};