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
        std::string command = "UPDATE users SET password=$1 WHERE login = $2";
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

bool sp2p::tracker::db::PsqlConnector::userExists(std::string &login) {
try{
    std::string transId = "user_exists_" + sp2p::tracker::utils::getRandomString(10);
    std::string command = "SELECT id FROM users WHERE login=$1";
    pqxx::work Xaction(*connection);
    connection->prepare(transId, command);
    pqxx::result res = Xaction.prepared(transId)(login).exec();
    Xaction.commit();
    return res.size() == 1;
}catch(pqxx::unique_violation e){
    std::cout << e.what();
    return false;
}
}

std::string PsqlConnector::getUserInfo(std::string &login, std::string &network) {
    std::string ret = "";
    try{
        std::string transId = "get_user_info_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT key FROM publickey WHERE user_id=(SELECT id FROM users WHERE login=$1)"
                "AND network_id=(SELECT id FROM networks WHERE name=$2)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login)(network).exec();
        Xaction.commit();
        if(res.size() == 0) return "";
        ret = utils::toString(res[0]["key"]);
    }catch(pqxx::failure e){
        std::cout << e.what();
        return ret;
    }
    return ret;
}

DB_Response PsqlConnector::addKey(std::string &login, std::string &network, std::string &key) {
    try{
        std::string transId = "add_key_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "INSERT INTO publickey (user_id, network_id, key) VALUES ("
                "(SELECT id FROM users WHERE login=$1), "
                "(SELECT id FROM networks WHERE name=$2), "
                "$3)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login)(network)(key).exec();
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


bool PsqlConnector::networkExists(std::string &name) {
    try{
        std::string transId = "network_exists_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT id FROM networks WHERE name=$1";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(name).exec();
        Xaction.commit();
        return res.size() > 0;
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
    return false;
}


bool PsqlConnector::canSeeNetwork(std::string &networkName, std::string &login) {
    try{
        std::string transId = "can_see_network_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT id FROM networks "
                "WHERE name=$2 AND (public=true OR "
                "owner_id=(SELECT id FROM users WHERE login=$1))";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login)(networkName).exec();
        Xaction.commit();
        return res.size() > 0;
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
    return false;
}

bool sp2p::tracker::db::PsqlConnector::canAddServer(std::string &login, std::string &networkName) {
    try{
        std::string transId = "can_add_server_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT id FROM networks "
                "WHERE name=$2 AND (participable=true OR "
                "owner_id=(SELECT id FROM users WHERE login=$1))";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(login)(networkName).exec();
        Xaction.commit();
        return res.size() > 0;
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
    return false;
}

DB_Response PsqlConnector::updateServer(std::string& network, std::string& user, std::string& ip, int port, int ttl) {
    try{

        //update

        std::string transId = "update_server_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "UPDATE TABLE servers SET ip=$3, port=$4, ttl=$5 "
                "WHERE user_id=(SELECT id FROM users WHERE login=$2) "
                "AND network_id=(SELECT id FROM networks WHERE name=$1)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(network)(user)(ip)(port)(ttl).exec();
        if(res.affected_rows() > 0)
            return DB_Response::OK;

        // insert if necessery
        transId = "update_server_" + sp2p::tracker::utils::getRandomString(10);
        command = "INSERT INTO servers(user_id, network_id, ip, port, ttl) "
                "values((SELECT id FROM networks WHERE name=$1), "
                "(SELECT id FROM users WHERE login=$2),  $3, $4, $5)";
        pqxx::work Xaction2(*connection);
        connection->prepare(transId, command);
        res = Xaction2.prepared(transId)(network)(user)(ip)(port)(ttl).exec();

    }catch(pqxx::unique_violation e){
        std::cout << e.what();
        return DB_Response::NOT_FOUND;
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

std::shared_ptr<std::list<utils::ProtoServer> > PsqlConnector::getServers(std::string &networkName) {
    std::shared_ptr<std::list<utils::ProtoServer> > ret(new std::list<utils::ProtoServer>());
    try{
        std::string transId = "get_servers_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT login, ip, port from servers s JOIN users u ON s.user_id=u.id " \
                "WHERE network_id=$1";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(networkName).exec();
        for(int i = 0; i < res.size(); ++i) {
            utils::ProtoServer currServer;
            currServer.setNetworkName(networkName);
            currServer.setIp(utils::toString(res[i]["ip"]));
            currServer.setPort(utils::toInt(res[i]["port"]));
            ret->push_back(currServer);
        }
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
    return ret;
}

void PsqlConnector::cleanServers(int ttl) {
    try{
        std::string transId = "clean_servers_" + sp2p::tracker::utils::getRandomString(10);
        std::string command = "SELECT clean($1)";
        pqxx::work Xaction(*connection);
        connection->prepare(transId, command);
        pqxx::result res = Xaction.prepared(transId)(ttl).exec();
        Xaction.commit();
    }catch(pqxx::unique_violation e){
        std::cout << e.what();
    }
}

} // namespace db
} // namespace tracker
} // namespace sp2p
