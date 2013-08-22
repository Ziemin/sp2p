#include "network.hpp"

Network::Network()
{
}

std::string Network::getName() const
{
    return name;
}

void Network::setName(const std::string &value)
{
    name = value;
}

bool Network::getPublicity() const
{
    return publicity;
}

void Network::setPublicity(bool value)
{
    publicity = value;
}

bool Network::getVisible() const
{
    return visible;
}

void Network::setVisible(bool value)
{
    visible = value;
}

bool Network::getParticipable() const
{
    return participable;
}

void Network::setParticipable(bool value)
{
    participable = value;
}

std::string Network::getCreatorEmail() const
{
    return creatorEmail;
}

void Network::setCreatorEmail(const std::string &value)
{
    creatorEmail = value;
}

std::string Network::getProtocolName() const
{
    return protocolName;
}

void Network::setProtocolName(const std::string &value)
{
    protocolName = value;
}

std::string Network::getOwnerName() const
{
    return ownerName;
}

void Network::setOwnerName(const std::string &value)
{
    ownerName = value;
}
