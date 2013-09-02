#include "proto_user.hpp"


namespace sp2p{
namespace tracker{
namespace utils{

ProtoUser::ProtoUser()
{
}

std::string ProtoUser::getLogin() const
{
    return login;
}

void ProtoUser::setLogin(const std::string &value)
{
    login = value;
}

std::string ProtoUser::getPassword() const
{
    return password;
}

void ProtoUser::setPassword(const std::string &value)
{
    password = value;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p
