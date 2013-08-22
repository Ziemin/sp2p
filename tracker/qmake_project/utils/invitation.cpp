#include "invitation.hpp"

Invitation::Invitation()
{
}

std::string Invitation::getUserLogin() const
{
    return userLogin;
}

void Invitation::setUserLogin(const std::string &value)
{
    userLogin = value;
}

std::string Invitation::getNetworkName() const
{
    return networkName;
}

void Invitation::setNetworkName(const std::string &value)
{
    networkName = value;
}
