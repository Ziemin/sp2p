#include "proto_network.hpp"

namespace sp2p{
namespace tracker{
namespace utils{

ProtoNetwork::ProtoNetwork()
{
}

std::string ProtoNetwork::getName() const
{
    return name;
}

void ProtoNetwork::setName(const std::string &value)
{
    name = value;
}

bool ProtoNetwork::getPublicity() const
{
    return publicity;
}

void ProtoNetwork::setPublicity(bool value)
{
    publicity = value;
}

bool ProtoNetwork::getVisible() const
{
    return visible;
}

void ProtoNetwork::setVisible(bool value)
{
    visible = value;
}

bool ProtoNetwork::getParticipable() const
{
    return participable;
}

void ProtoNetwork::setParticipable(bool value)
{
    participable = value;
}

std::string ProtoNetwork::getCreatorEmail() const
{
    return creatorEmail;
}

void ProtoNetwork::setCreatorEmail(const std::string &value)
{
    creatorEmail = value;
}

std::string ProtoNetwork::getProtocolName() const
{
    return protocolName;
}

void ProtoNetwork::setProtocolName(const std::string &value)
{
    protocolName = value;
}

std::string ProtoNetwork::getOwnerName() const
{
    return ownerName;
}

void ProtoNetwork::setOwnerName(const std::string &value)
{
    ownerName = value;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p
