#pragma once

#include "user.hpp"

class GroupUser : public User
{
public:
    void setRole(string role) { this->role = role; }
    string getrole() { return this->role; }

private:
    string role;
};