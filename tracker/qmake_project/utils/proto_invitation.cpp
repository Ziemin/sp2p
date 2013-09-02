#include "proto_invitation.hpp"

namespace sp2p{
namespace tracker{
namespace utils{

ProtoInvitation::ProtoInvitation()
{
}

std::string ProtoInvitation::getUserLogin() const
{
    return userLogin;
}

void ProtoInvitation::setUserLogin(const std::string &value)
{
    userLogin = value;
}

std::string ProtoInvitation::getNetworkName() const
{
    return networkName;
}

void ProtoInvitation::setNetworkName(const std::string &value)
{
    networkName = value;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p
