#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <string>

#include "dbresponse.h"

class AbstractDBConnector {
public:

    virtual ~AbstractDBConnector() = default;

    virtual DB_Response createUser(std::string& login, std::string& password)=0;
    virtual DB_Response removeUser(std::string& login)=0;
    virtual DB_Response removeUser(int id)=0;

    virtual DB_Response createInvitation(std::string& login, std::string& networkName)=0;
    virtual DB_Response deleteInvitation(std::string& login, std::string& networkName)=0;

    virtual DB_Response createNetwork(std::string& networkName, std::string& ownerLogin, bool publicity, \
                                      bool visible, bool participable, std::string& creatorMail, \
                                      std::string& protocolName)=0;
    virtual DB_Response deleteNetwork(std::string& name)=0;

    virtual DB_Response updateServer(std::string &network, std::string &user, std::string& ip, \
                                     int port, int ttl)=0;
    virtual DB_Response deleteServer(std::string &network, std::string& user)=0;

};

#endif // DATABASECONNECTOR_H
