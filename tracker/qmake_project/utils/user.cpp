#include "user.hpp"

User::User()
{
}

std::string User::getLogin() const
{
    return login;
}

void User::setLogin(const std::string &value)
{
    login = value;
}

std::string User::getPassword() const
{
    return password;
}

void User::setPassword(const std::string &value)
{
    password = value;
}
