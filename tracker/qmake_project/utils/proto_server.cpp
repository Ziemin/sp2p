#include "proto_server.hpp"

namespace sp2p{
namespace tracker{
namespace utils{

ProtoServer::ProtoServer()
{
}

std::string ProtoServer::getUserLogin() const
{
    return userLogin;
}

void ProtoServer::setUserLogin(const std::string &value)
{
    userLogin = value;
}

std::string ProtoServer::getNetworkName() const
{
    return networkName;
}

void ProtoServer::setNetworkName(const std::string &value)
{
    networkName = value;
}

std::string ProtoServer::getIp() const
{
    return ip;
}

void ProtoServer::setIp(const std::string &value)
{
    ip = value;
}

int ProtoServer::getPort() const
{
    return port;
}

void ProtoServer::setPort(int value)
{
    port = value;
}

long long ProtoServer::getTtl() const
{
    return ttl;
}

void ProtoServer::setTtl(long long value)
{
    ttl = value;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p
