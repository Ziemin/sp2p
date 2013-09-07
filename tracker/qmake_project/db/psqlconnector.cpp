#include "psqlconnector.h"
#include "utils/util_functions.h"

#include <pqxx/connection>
#include <pqxx/connectionpolicy>
#include <pqxx/transaction>
#include <pqxx/transactor>
#include <iostream>


namespace sp2p {
namespace tracker {
namespace db {

PsqlConnector::PsqlConnector(std::string dbName, std::string dbUser, std::string dbPassword, \
                             std::string dbAdress, std::string dbPort) :
    AbstractDBConnector(),
    dbName(dbName),
    dbUser(dbUser),
    dbPassword(dbPassword),
    dbAdress(dbAdress),
    dbPort(dbPort)
{
    std::string config = \
            "dbname=" + dbName + " " \
            "user=" + dbUser + " " \
            "password=" + dbPassword + " " \
            "hostaddr=" + dbAdress + " " \
            "port=" + dbPort;
    connection = new pqxx::connection(config);
}

PsqlConnector::~PsqlConnector() {
    delete connection;
}


DB_Response PsqlConnector::createUser(std::string& login, std::string& password) {
    try{
        std::string transId = "create_user_" + sp2p::tracker::utils::getRandomString(10);
        pqxx::work Xaction(*connection);
        connection->prepare(transId, "INSERT INTO users(login, password) VALUES($1, $2)");
        pqxx::result res = Xaction.prepared(transId)(login)(password).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_UNIQUE;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::removeUser(std::string& login) {
    try{
        std::string transId = "remove_user_" + sp2p::tracker::utils::getRandomString(10);
        pqxx::work Xaction(*connection);
        connection->prepare(transId, "DELETE FROM users WHERE login=$1");
        pqxx::result res = Xaction.prepared(transId)(login).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::removeUser(int id) {
    try{
        std::string transId = "remove_user_" + sp2p::tracker::utils::getRandomString(10);
        pqxx::work Xaction(*connection);
        connection->prepare(transId, "DELETE FROM users WHERE id=$1");
        pqxx::result res = Xaction.prepared(transId)(id).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
    }
    return DB_Response::OK;
}

bool sp2p::tracker::db::PsqlConnector::isUser(std::string &login, std::string &password) {
    try{
        std::string transId = "is_user_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT id FROM users WHERE login=$1 AND password=$2";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login)(password).exec();
        Xaction.commit();
        return res.size() == 1;
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return false;
    }
}

sp2p::tracker::db::DB_Response sp2p::tracker::db::PsqlConnector::changeUserPassword(std::string &login, std::string &newPassword) {
    try{
        std::string transId = "change_password_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "UPDATE users SET password=$1 WHERE login = $2;";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(newPassword)(login).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::createInvitation(std::string& login, std::string& networkName) {
    try{
        std::string transId = "create_invitation_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "INSERT INTO invitations(network_id, user_id) "\
                "values((SELECT id FROM networks WHERE name=$1), " \
                "(SELECT id FROM users WHERE login=$2))";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(networkName)(login).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_UNIQUE;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::deleteInvitation(std::string& login, std::string& networkName) {
    try{
        std::string transId = "remove_invitation_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "DELETE FROM invitations WHERE "\
                "network_id=(SELECT id FROM networks WHERE name=$1) AND " \
                "user_id=(SELECT id FROM users WHERE login=$2)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(networkName)(login).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::createNetwork(std::string& networkName, std::string& ownerLogin, bool publicity, \
                                         bool visible, bool participable, std::string& creatorMail, \
                                         std::string& protocolName) {
    try{
        std::string transId = "create_network_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "INSERT INTO networks(name, public, visible, participable, creator_email, protocol_name, owner_id) "\
                "values($1, $2, $3, $4, $5, $6, (SELECT id FROM users WHERE login=$7))";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(networkName)(publicity)(visible)(participable)(creatorMail)(protocolName)(ownerLogin).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_UNIQUE;
    }catch(pqxx::foreign_key_violation e){
        std::cout << e.what();
        return DB_Response::NOT_UNIQUE;
    }

    return DB_Response::OK;
}

DB_Response PsqlConnector::deleteNetwork(std::string &login, std::string& name) {
    try{
        std::string transId = "remove_network_" + sp2p::tracker::utils::getRandomString(10);
        pqxx::work Xaction(*connection);
        connection->prepare(transId, "DELETE FROM networks WHERE name=$1 AND "
                            "owner_id = (SELECT id FROM users WHERE login=$2)");
        pqxx::result res = Xaction.prepared(transId)(name)(login).exec();
        Xaction.commit();
        if(res.affected_rows() == 0) return db::NOT_PERMITED;
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::INTERNAL_ERROR;
    }
    return DB_Response::OK;
}

std::shared_ptr<std::list<utils::ProtoNetwork> >
PsqlConnector::getAllNetworks(std::string &login) {
    std::shared_ptr<std::list<utils::ProtoNetwork> > ret(new std::list<utils::ProtoNetwork>());
    try{
        std::string transId = "get_all_networks_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT name, login, creator_email, visible, participable, protocol_name "\
                "FROM networks n JOIN users u ON n.owner_id = u.id WHERE visible=true OR login=$1 OR "\
                "n.id IN (SELECT network_id FROM invitations WHERE user_id = (SELECT id FROM users WHERE login=$1))";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login).exec();
        for(int i = 0; i < res.size(); ++i) {
            utils::ProtoNetwork currentNetwork;
            currentNetwork.setName(utils::toString(res[i]["name"]));
            currentNetwork.setOwnerName(utils::toString(res[i]["login"]));
            currentNetwork.setCreatorEmail(utils::toString(res[i]["creator_email"]));
            currentNetwork.setVisible(utils::toBool(res[i]["visible"]));
            currentNetwork.setParticipable(utils::toBool(res[i]["participable"]));
            currentNetwork.setProtocolName(utils::toString(res[i]["protocol_name"]));

            ret->push_back(currentNetwork);
        }
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
    return ret;
}

std::shared_ptr<std::list<utils::ProtoNetwork> > sp2p::tracker::db::PsqlConnector::getUserNetworks(std::string &login) {
    std::shared_ptr<std::list<utils::ProtoNetwork> > ret(new std::list<utils::ProtoNetwork>());
    try{
        std::string transId = "get_all_networks_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT name, login, creator_email, visible, participable, protocol_name "\
                "FROM networks n JOIN users u ON n.owner_id = u.id WHERE login=$1";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login).exec();
        for(int i = 0; i < res.size(); ++i) {
            utils::ProtoNetwork currentNetwork;
            currentNetwork.setName(utils::toString(res[i]["name"]));
            currentNetwork.setOwnerName(utils::toString(res[i]["login"]));
            currentNetwork.setCreatorEmail(utils::toString(res[i]["creator_email"]));
            currentNetwork.setVisible(utils::toBool(res[i]["visible"]));
            currentNetwork.setParticipable(utils::toBool(res[i]["participable"]));
            currentNetwork.setProtocolName(utils::toString(res[i]["protocol_name"]));

            ret->push_back(currentNetwork);
        }
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
    return ret;
}


DB_Response PsqlConnector::updateServer(std::string& network, std::string& user, std::string& ip, int port, int ttl) {
    try{
        this->deleteServer(network, user);
        std::string transId = "update_server_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "INSERT INTO servers(user_id, network_id, ip, port, ttl) "\
                "values((SELECT id FROM networks WHERE name=$1), " \
                "(SELECT id FROM users WHERE login=$2), "\
                "$3, $4, $5)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(network)(user)(ip)(port)(ttl).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_UNIQUE;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::deleteServer(std::string& network, std::string& user) {
    try{
        std::string transId = "remove_server_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "DELETE FROM servers WHERE "\
                "network_id=(SELECT id FROM networks WHERE name=$1 ) AND " \
                "user_id=(SELECT id FROM users WHERE login=$2)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(network)(user).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
    }
    return DB_Response::OK;
}

} // namespace db
} // namespace tracker
} // namespace sp2p





