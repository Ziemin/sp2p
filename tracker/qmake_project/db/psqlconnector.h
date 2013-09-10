#ifndef PSQLCONNECTOR_H
#define PSQLCONNECTOR_H

#include <string>
#include <memory>
#include <list>
#include <pqxx/pqxx>

#include "abstractdbconnector.h"
#include "dbresponse.h"

namespace sp2p {
namespace tracker {
namespace db {

class PsqlConnector : public AbstractDBConnector
{
public:

    PsqlConnector(std::string dbName, std::string dbUser, std::string dbPassword, \
                  std::string dbAdress="127.0.0.1", std::string dbPort="5432");
    ~PsqlConnector();

    DB_Response createUser(std::string& login, std::string& password);
    DB_Response removeUser(std::string& login);
    DB_Response removeUser(int id);
    bool isUser(std::string &login, std::string &password);
    DB_Response changeUserPassword(std::string &login, std::string &newPassword);
    bool userExists(std::string &login);
    std::string getPassHash(const std::string &login);

    std::string getUserInfo(std::string &login, std::string &network);
    DB_Response addKey(std::string &login, std::string &network, std::string &key);

    DB_Response createInvitation(std::string& login, std::string& networkName);
    DB_Response deleteInvitation(std::string& login, std::string& networkName);

    DB_Response createNetwork(std::string& networkName, std::string& ownerLogin, bool publicity, \
                              bool visible, bool participable, std::string& creatorMail, \
                              std::string& protocolName);
    DB_Response deleteNetwork(std::string &login, std::string& name);
    std::shared_ptr<std::list<utils::ProtoNetwork>> getAllNetworks(std::string &login);
    std::shared_ptr<std::list<utils::ProtoNetwork>> getUserNetworks(std::string &login);
    bool networkExists(std::string &name);

    bool canAddServer(std::string &login, std::string &networkName);

    bool canSeeNetwork(std::string &networkName, std::string &login);
    DB_Response updateServer(std::string &network, std::string &user, std::string& ip, \
                             int port, int ttl);
    DB_Response deleteServer(std::string &network, std::string& user);
    std::shared_ptr<std::list<utils::ProtoServer> > getServers(std::string &networkName);
    void cleanServers(int ttl);

private:
    std::string dbName;
    std::string dbUser;
    std::string dbPassword;
    std::string dbAdress;
    std::string dbPort;

    pqxx::connection* connection;

};

} // namespace db
} // namespace tracker
} // namespace sp2p

#endif // PSQLCONNECTOR_H
