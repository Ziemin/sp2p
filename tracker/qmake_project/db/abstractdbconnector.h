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
/**
 * @brief The AbstractDBConnector class - abstract class defining connection with DB
 * Override it to specify connection method, driver etc.
 */
class AbstractDBConnector {
public:

    virtual ~AbstractDBConnector() = default;

    /**
     * @brief createUser - Adds new user to DB
     * @param login - new user login
     * @param password - new user password
     * @return DB_Response with execution status
     */
    virtual DB_Response createUser(std::string& login, std::string& password)=0;

    /**
     * @brief removeUser removes user with given login
     * @param login - user to remove
     * @return DB_Response with execution status
     */
    virtual DB_Response removeUser(std::string& login)=0;

    /**
     * @brief removeUser - removes user
     * @param id - user id in DB
     * @return DB_Response with execution status
     */
    virtual DB_Response removeUser(int id)=0;

    /**
     * @brief isUser - checks if user exists
     * @param login
     * @param password
     * @return true if and only if there is an user with given login and password
     */
    virtual bool isUser(std::string& login, std::string& password)=0;

    /**
     * @brief changeUserPassword - changes user password
     * @param login - user login
     * @param newPassword - new password to be set
     * @return DB_Response with execution status
     */
    virtual DB_Response changeUserPassword(std::string& login, std::string& newPassword)=0;

    /**
     * @brief userExists - checks if there is a user with given login
     * @param login
     * @return true if and only if there is a user with given login
     */
    virtual bool userExists(std::string &login)=0;

    /**
     * @brief getPassHash
     * @param login - user login
     * @return string stored in password field of given user or empty string if doesn't exist
     */
    virtual std::string getPassHash(const std::string &login)=0;

    /**
     * @brief getUserInfo
     * @param login
     * @param network
     * @return user public key in given network or empty string if not found
     */
    virtual std::string getUserInfo(std::string &login, std::string &network)=0;

    /**
     * @brief addKey - adds users public key to given network
     * @param login
     * @param network
     * @param key
     * @return DB_Response with execution status
     */
    virtual DB_Response addKey(std::string &login, std::string &network, std::string &key)=0;

    /**
     * @brief createInvitation - adds user to network
     * @param login
     * @param networkName
     * @return DB_Response with execution status
     */
    virtual DB_Response createInvitation(std::string& login, std::string& networkName)=0;

    /**
     * @brief deleteInvitation - removes user from network
     * @param login
     * @param networkName
     * @return DB_Response with execution status
     */
    virtual DB_Response deleteInvitation(std::string& login, std::string& networkName)=0;

    /**
     * @brief createNetwork - creates a new network
     * @param networkName
     * @param ownerLogin
     * @param publicity
     * @param visible
     * @param participable
     * @param creatorMail
     * @param protocolName
     * @return DB_Response with execution status
     */
    virtual DB_Response createNetwork(std::string& networkName, std::string& ownerLogin, bool publicity, \
                                      bool visible, bool participable, std::string& creatorMail, \
                                      std::string& protocolName)=0;

    /**
     * @brief deleteNetwork - deletes a network
     * @param login
     * @param name
     * @return DB_Response with execution status
     */
    virtual DB_Response deleteNetwork(std::string &login, std::string& name)=0;

    /**
     * @brief getAllNetworks - getter for all network visible by given user
     * @param login
     * @return list of networks
     */
    virtual std::shared_ptr<std::list<utils::ProtoNetwork>> getAllNetworks(std::string &login)=0;

    /**
     * @brief getUserNetworks - getter for all user's networks
     * @param login
     * @return list of networks
     */
    virtual std::shared_ptr<std::list<utils::ProtoNetwork>> getUserNetworks(std::string &login)=0;

    /**
     * @brief networkExists - checks if network exists
     * @param name
     * @return true if and anly if given network exists
     */
    virtual bool networkExists(std::string &name)=0;

    /**
     * @brief canSeeNetwork - checks if user can see given network
     * @param networkName
     * @param login
     * @return true if and only if user can see the network
     */
    virtual bool canSeeNetwork(std::string &networkName, std::string &login)=0;

    /**
     * @brief canAddServer - checks if user can create server in given network
     * @param login
     * @param networkName
     * @return true if and only if user can create server in the network
     */
    virtual bool canAddServer(std::string &login, std::string &networkName)=0;

    /**
     * @brief updateServer - creates a server or updates existing
     * @param network
     * @param user
     * @param ip
     * @param port
     * @param ttl - unix time defining when the server has to be removed
     * @return DB_Response with execution status
     */
    virtual DB_Response updateServer(std::string &network, std::string &user, std::string& ip, \
                                     int port, int ttl)=0;

    /**
     * @brief deleteServer - deletes a server
     * @param network
     * @param user
     * @return DB_Response with execution status
     */
    virtual DB_Response deleteServer(std::string &network, std::string& user)=0;

    /**
     * @brief getServers - getter for all servers of given network
     * @param networkName
     * @return list of networks servers
     */
    virtual std::shared_ptr<std::list<utils::ProtoServer> > getServers(std::string &networkName)=0;

    /**
     * @brief cleanServers - removes all outdated servers
     * @param ttl - unix time. Servers will lower ttl will be removed
     */
    virtual void cleanServers(int ttl)=0;

};

} // namespace db
} // namespace tracker
} // namespace sp2p

#endif // DATABASECONNECTOR_H
