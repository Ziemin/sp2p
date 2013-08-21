#ifndef PSQLCONNECTOR_H
#define PSQLCONNECTOR_H

#include <string>
#include <pqxx/pqxx>

#include "abstractdbconnector.h"
#include "dbresponse.h"

class PsqlConnector : public AbstractDBConnector
{
public:

    PsqlConnector(std::string dbName, std::string dbUser, std::string dbPassword, \
                  std::string dbAdress="127.0.0.1", std::string dbPort="5432");
    virtual ~PsqlConnector();

    virtual DB_Response createUser(std::string& login, std::string& password);
    virtual DB_Response removeUser(std::string& login);
    virtual DB_Response removeUser(int id);

    virtual DB_Response createInvitation(std::string& login, std::string& networkName);
    virtual DB_Response deleteInvitation(std::string& login, std::string& networkName);

    virtual DB_Response createNetwork(std::string& networkName, std::string& ownerLogin, bool publicity, \
                                      bool visible, bool participable, std::string& creatorMail, \
                                      std::string& protocolName);
    virtual DB_Response deleteNetwork(std::string& name);

    virtual DB_Response updateServer(std::string &network, std::string &user, std::string& ip, \
                              int port, int ttl);
    virtual DB_Response deleteServer(std::string &network, std::string& user);

private:
    std::string dbName;
    std::string dbUser;
    std::string dbPassword;
    std::string dbAdress;
    std::string dbPort;

    pqxx::connection* connection;

};

#endif // PSQLCONNECTOR_H
