#include "server.hpp"

Server::Server()
{
}

std::string Server::getUserLogin() const
{
    return userLogin;
}

void Server::setUserLogin(const std::string &value)
{
    userLogin = value;
}

std::string Server::getNetworkName() const
{
    return networkName;
}

void Server::setNetworkName(const std::string &value)
{
    networkName = value;
}

std::string Server::getIp() const
{
    return ip;
}

void Server::setIp(const std::string &value)
{
    ip = value;
}

int Server::getPort() const
{
    return port;
}

void Server::setPort(int value)
{
    port = value;
}

long long Server::getTtl() const
{
    return ttl;
}

void Server::setTtl(long long value)
{
    ttl = value;
}
