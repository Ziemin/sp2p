#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <string>
#include <list>

#include "dbresponse.h"
#include "utils/proto_network.hpp"
#include "utils/proto_server.hpp"

namespace sp2p {
namespace tracker {
namespace db {

class AbstractDBConnector {
public:

    virtual ~AbstractDBConnector() = default;

    virtual DB_Response createUser(std::string& login, std::string& password)=0;
    virtual DB_Response removeUser(std::string& login)=0;
    virtual DB_Response removeUser(int id)=0;
    virtual bool isUser(std::string& login, std::string& password)=0;
    virtual DB_Response changeUserPassword(std::string& login, std::string& newPassword)=0;
    virtual bool userExists(std::string &login)=0;

    virtual std::string getUserInfo(std::string &login, std::string &network)=0;
    virtual DB_Response addKey(std::string &login, std::string &network, std::string &key)=0;

    virtual DB_Response createInvitation(std::string& login, std::string& networkName)=0;
    virtual DB_Response deleteInvitation(std::string& login, std::string& networkName)=0;

    virtual DB_Response createNetwork(std::string& networkName, std::string& ownerLogin, bool publicity, \
                                      bool visible, bool participable, std::string& creatorMail, \
                                      std::string& protocolName)=0;
    virtual DB_Response deleteNetwork(std::string &login, std::string& name)=0;
    virtual std::shared_ptr<std::list<utils::ProtoNetwork>> getAllNetworks(std::string &login)=0;
    virtual std::shared_ptr<std::list<utils::ProtoNetwork>> getUserNetworks(std::string &login)=0;
    virtual bool networkExists(std::string &name)=0;
    virtual bool canSeeNetwork(std::string &networkName, std::string &login)=0;

    virtual bool canAddServer(std::string &login, std::string &networkName)=0;

    virtual DB_Response updateServer(std::string &network, std::string &user, std::string& ip, \
                                     int port, int ttl)=0;
    virtual DB_Response deleteServer(std::string &network, std::string& user)=0;
    virtual std::shared_ptr<std::list<utils::ProtoServer> > getServers(std::string &networkName)=0;

    virtual void cleanServers(int ttl)=0;

};

} // namespace db
} // namespace tracker
} // namespace sp2p

#endif // DATABASECONNECTOR_H
