#include "psqlconnector.h"
#include "utils.h"

#include <pqxx/connection>
#include <pqxx/connectionpolicy>
#include <pqxx/transaction>
#include <pqxx/transactor>
#include <iostream>

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
        std::string transId = "create_user_" + db_utils::getRandomString(10);
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
        std::string transId = "remove_user_" + db_utils::getRandomString(10);
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
        std::string transId = "remove_user_" + db_utils::getRandomString(10);
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

DB_Response PsqlConnector::createInvitation(std::string& login, std::string& networkName) {
    try{
        std::string transId = "create_invitation_" + db_utils::getRandomString(10);
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
        std::string transId = "remove_invitation_" + db_utils::getRandomString(10);
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
        std::string transId = "create_network_" + db_utils::getRandomString(10);
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

DB_Response PsqlConnector::deleteNetwork(std::string& name) {
    try{
        std::string transId = "remove_network_" + db_utils::getRandomString(10);
        pqxx::work Xaction(*connection);
        connection->prepare(transId, "DELETE FROM networks WHERE name=$1");
        pqxx::result res = Xaction.prepared(transId)(name).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
    }
    return DB_Response::OK;
}

DB_Response PsqlConnector::updateServer(std::string& network, std::string& user, std::string& ip, int port, int ttl) {
    try{
        this->deleteServer(network, user);
        std::string transId = "update_server_" + db_utils::getRandomString(10);
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
        std::string transId = "remove_server_" + db_utils::getRandomString(10);
        std::string command = "DELETE FROM servers WHERE "\
                "network_id=(SELECT id FROM networks WHERE name=$1 ) ANS " \
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







