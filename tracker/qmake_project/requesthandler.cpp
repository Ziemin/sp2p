#include "requesthandler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "proto/sp2p_protocol.pb.h"
#include "request.hpp"
#include "response.hpp"
#include "proto/nodemessagefactory.h"
#include "constants.h"
#include "utils/util_functions.h"

namespace sp2p {
namespace tracker {

RequestHandler::RequestHandler(SessionControler_ptr sessionControler, DBConnector_ptr DBConnector_, boost::asio::ip::address address_)
{
    this->sessionControler = sessionControler;
    this->DBConnector_ = DBConnector_;
    this->address_ = address_;
}

void RequestHandler::handleRequest(const protocol_factory::AbstractRequest *req, protocol_factory::AbstractResponse *rep) const
{
    Request *request = (Request*) req;
    Response *response = (Response*) rep;

    const protocol::ClientMessage *clientMessage = request->getMessage();
    protocol::NodeMessage *nodeMessage = response->getMessage();

    NodeMessage_ptr answer;

    switch(clientMessage->request()) {
    case protocol::ClientMessage::REGISTER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "REGISTER";
#endif
        answer = handleRegister(clientMessage);
        break;
    case protocol::ClientMessage::UNREGISTER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "UNREGISTER";
#endif
        answer = handleUnregister(clientMessage);
        break;
    case protocol::ClientMessage::LOGIN:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LOGIN";
#endif
        answer = handleLogin(clientMessage);
        break;
    case protocol::ClientMessage::LOGOUT:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LOGOUT";
#endif
        answer = handleLogout(clientMessage);
        break;
    case protocol::ClientMessage::LIST_NETWORKS:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LIST_NETWORKS";
#endif
        answer = handleList_networks(clientMessage);
        break;
    case protocol::ClientMessage::LIST_MY_NETWORKS:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LIST_MY_NETWORKS";
#endif
        answer = handleList_my_networks(clientMessage);
        break;
    case protocol::ClientMessage::LIST_SERVERS:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LIST_SERVERS";
#endif
        answer = handleList_servers(clientMessage);
        break;
    case protocol::ClientMessage::CREATE_NETWORK:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "CREATE_NETWORK";
#endif
        answer = handleCreate_network(clientMessage);
        break;
    case protocol::ClientMessage::DELETE_NETWORK:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "DELETE_NETWORK";
#endif
        answer = handleDelete_network(clientMessage);
        break;
    case protocol::ClientMessage::INVITE_USER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "INVITE_USER";
#endif
        answer = handleInvite_user(clientMessage);
        break;
    case protocol::ClientMessage::REMOVE_USER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "REMOVE_USER";
#endif
        answer = handleRemove_user(clientMessage);
        break;
    case protocol::ClientMessage::USER_INFO:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "USER_INFO";
#endif
        answer = handleUser_info(clientMessage);
        break;
    case protocol::ClientMessage::UPDATE_SERVER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "UPDATE_SERVER";
#endif
        answer = handleUpdate_server(clientMessage);
        break;
    case protocol::ClientMessage::STOP_SERVER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "STOP_SERVER";
#endif
        answer = handleStop_server(clientMessage);
        break;
    case protocol::ClientMessage::SIGN_KEY:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "SIGN_KEY";
#endif
        answer = handleSign_key(clientMessage);
        break;
    case protocol::ClientMessage::CHANGE_PASSWORD:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "CHANGE_PASSWORD";
#endif
        answer = handleChange_password(clientMessage);
        break;
    default:
        answer = NodeMessageFactory::badRequestMessage();
        break;
    }

    *nodeMessage = *answer.get();
}

void RequestHandler::badRequestResponse(protocol_factory::AbstractResponse *rep) const {
    auto badRequest = NodeMessageFactory::badRequestMessage();
    protocol::NodeMessage * nodeMessage = ((Response*) rep)->getMessage();
    *nodeMessage = *badRequest.get();
}

NodeMessage_ptr RequestHandler::handleRegister(const protocol::ClientMessage *clientMessage) const {

    // TO DO

    if(clientMessage->has_register_message()) {
        if(clientMessage->register_message().has_username()
                && clientMessage->register_message().has_password()
                && clientMessage->register_message().has_public_key()) {
            std::string login = clientMessage->register_message().username();
            std::string pass = clientMessage->register_message().password();
            std::string hash = utils::getPasswordHash(pass);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Register login: " << login << " password: " << pass << " hash: " << hash;
#endif
            db::DB_Response db_response = DBConnector_->createUser(login, hash);
            switch(db_response) {
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
            case db::NOT_PERMITED:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            case db::NOT_UNIQUE:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
            case db::OK:
                std::string cert = "notimplementedyet";
                std::string nodeCert = "dupadupa";
                return NodeMessageFactory::registerResponse(cert, nodeCert);
            }
        }
    }

    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleUnregister(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_unregister_message()) {
        if(clientMessage->unregister_message().has_cookie()) {
            std::string cookie = clientMessage->unregister_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

            db::DB_Response db_response = DBConnector_->removeUser(login);
            switch(db_response) {
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
            case db::NOT_PERMITED:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            case db::NOT_UNIQUE:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
            case db::OK:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            }
        }
    }

    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleLogin(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_login_message()) {
        if(clientMessage->login_message().has_password()
                && clientMessage->login_message().has_username()) {
            std::string login = clientMessage->login_message().username();
            std::string pass = clientMessage->login_message().password();
            bool goodPass = goodCredentials(login, pass);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Login: " << login << " pass: " << pass << " exists: " << goodPass;
#endif
            if(!goodPass)
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::BAD_CREDENTIALS);
            std::string cookie = sessionControler->login(login);
            std::string cert = "notimplementedyet";
            return NodeMessageFactory::loginResponse(cookie, cert);
        }
    }

    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleLogout(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_logout_message()) {
        if(clientMessage->logout_message().has_cookie()) {
            std::string cookie = clientMessage->logout_message().cookie();
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie;
#endif
            if(sessionControler->logout(cookie))
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleList_networks(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_list_networks_message()) {
        if(clientMessage->list_networks_message().has_cookie()) {
            std::string cookie = clientMessage->list_networks_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            auto L = DBConnector_->getAllNetworks(login);
            if(L == nullptr)
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
#ifdef DEBUG_LOGGING
            std::string networks = "Networks: ";
            for(auto i = L->begin(); i != L->end(); ++i)
                networks += i->getName() + ", ";
            BOOST_LOG_TRIVIAL(debug) << networks;
#endif
            return NodeMessageFactory::listNetworksResponse(L);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleList_my_networks(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_list_my_networks_message()) {
        if(clientMessage->list_my_networks_message().has_cookie()) {
            std::string cookie = clientMessage->list_my_networks_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            auto L = DBConnector_->getUserNetworks(login);
#ifdef DEBUG_LOGGING
            std::string networks = "Networks: ";
            for(auto i = L->begin(); i != L->end(); ++i)
                networks += i->getName();
            BOOST_LOG_TRIVIAL(debug) << networks;
#endif
            return NodeMessageFactory::listMyNetworksResponse(L);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleList_servers(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_list_servers_message()) {
        if(clientMessage->list_servers_message().has_cookie()
                && clientMessage->list_servers_message().has_network_name()) {
            std::string cookie = clientMessage->list_servers_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            std::string network = clientMessage->list_servers_message().network_name();
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Network: " << network;
#endif
            if(!DBConnector_->networkExists(network)) {
#ifdef DEBUG_LOGGING
                BOOST_LOG_TRIVIAL(debug) << "Network does not exist";
#endif
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
            }
            if(!DBConnector_->canSeeNetwork(network, login)) {
#ifdef DEBUG_LOGGING
                BOOST_LOG_TRIVIAL(debug) << "Not permited to see network";
#endif
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            }
            auto L = DBConnector_->getServers(network);
#ifdef DEBUG_LOGGING
            std::string networks = "Servers: ";
            for(auto i = L->begin(); i != L->end(); ++i)
                networks += "[ " + i->getUserLogin() + ", " + i->getNetworkName() + ", " +
                        i->getIp() + ", " + std::to_string(i->getPort()) + "] ";
            BOOST_LOG_TRIVIAL(debug) << networks;
#endif
            return NodeMessageFactory::listServersResponse(L);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleCreate_network(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_create_network_message()) {
        if(clientMessage->create_network_message().has_cookie() &&
                clientMessage->create_network_message().has_network_name() &&
                clientMessage->create_network_message().has_access_rights() &&
                clientMessage->create_network_message().has_visibility() &&
                clientMessage->create_network_message().has_participation_rights()) {
            std::string cookie = clientMessage->create_network_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

            std::string name = clientMessage->create_network_message().network_name();
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Network name: " << name;
#endif
            bool accessRights = (clientMessage->create_network_message().access_rights()
                                 == protocol::ClientMessage::CreateNetwork::PUBLIC);
            bool visible = (clientMessage->create_network_message().visibility()
                            == protocol::ClientMessage::CreateNetwork::LISTED);
            bool participable = (clientMessage->create_network_message().participation_rights()
                                 == protocol::ClientMessage::CreateNetwork::CLIENT_SERVER);
            std::string protocol = "";
            if(clientMessage->create_network_message().has_protocol_name())
                protocol = clientMessage->create_network_message().protocol_name();
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Protocol: " << protocol;
#endif
            std::string ownerEmail = "";
            if(clientMessage->create_network_message().has_creator_email())
                ownerEmail = clientMessage->create_network_message().creator_email();
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Email: " << ownerEmail;
#endif
            db::DB_Response db_response = DBConnector_->createNetwork(name, login, accessRights,
                                                                      visible, participable, ownerEmail,
                                                                      protocol);
            switch(db_response) {
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
            case db::NOT_PERMITED:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            case db::NOT_UNIQUE:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
            case db::OK:
                DBConnector_->createInvitation(login, name);
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            }
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleDelete_network(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_delete_network_message()) {
        if(clientMessage->delete_network_message().has_cookie()
                && clientMessage->delete_network_message().has_name()) {
            std::string cookie = clientMessage->delete_network_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            std::string name = clientMessage->delete_network_message().name();
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Network: " << name;
#endif
            if(!DBConnector_->networkExists(name)) {
#ifdef DEBUG_LOGGING
                BOOST_LOG_TRIVIAL(debug) << "Network " << name << " does not exist";
#endif

                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
            }
            db::DB_Response db_response = DBConnector_->deleteNetwork(login, name);
            switch(db_response) {
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
            case db::NOT_PERMITED:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            case db::NOT_UNIQUE:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
            case db::OK:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            }
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleInvite_user(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_invite_user_message()) {
        if(clientMessage->invite_user_message().has_cookie()
                && clientMessage->invite_user_message().has_network_name()
                && clientMessage->invite_user_message().has_username()) {

            std::string cookie = clientMessage->invite_user_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

            std::string invitedLogin = clientMessage->invite_user_message().username();
            std::string network = clientMessage->invite_user_message().network_name();

#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Inviting: " << invitedLogin << " to network: " << network;
#endif
            if(!DBConnector_->networkExists(network)) {
#ifdef DEBUG_LOGGING
                BOOST_LOG_TRIVIAL(debug) << "Network " << network << " does not exist";
#endif

                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
            }

            if(!DBConnector_->userExists(invitedLogin)) {
#ifdef DEBUG_LOGGING
                BOOST_LOG_TRIVIAL(debug) << "User " << invitedLogin << " does not exist";
#endif

                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
            }

            db::DB_Response db_response = DBConnector_->createInvitation(invitedLogin, network);
            switch(db_response) {
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
            case db::NOT_PERMITED:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            case db::NOT_UNIQUE:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
            case db::OK:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            }
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleRemove_user(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_remove_user_message()) {
        std::string cookie = clientMessage->remove_user_message().cookie();
        std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
        if(login == "")
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
        db::DB_Response db_response = DBConnector_->removeUser(login);
        sessionControler->logout(cookie);
        switch(db_response) {
        case db::INTERNAL_ERROR:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        case db::NOT_FOUND:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
        case db::NOT_PERMITED:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
        case db::NOT_UNIQUE:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
        case db::OK:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
        default:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        }
    }

    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleUser_info(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_user_info_message()) {
        std::string cookie = clientMessage->user_info_message().cookie();
        std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
        if(login == "")
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

        std::string infoLogin = clientMessage->user_info_message().username();
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Asking about: " << infoLogin;
#endif
        if(!DBConnector_->userExists(infoLogin)) {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "User " << infoLogin << " does not exist";
#endif
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
        }

        std::string network = clientMessage->user_info_message().network_name();
        if(!DBConnector_->networkExists(network)) {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Network " << network << " does not exist";
#endif
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
        }
        std::string key = DBConnector_->getUserInfo(infoLogin, network);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Signed key length: " << key.length();
#endif
        return NodeMessageFactory::userInfoResponse(key);
    }

    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleUpdate_server(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_update_server_message()) {
        std::string cookie = clientMessage->update_server_message().cookie();
        std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
        if(login == "")
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

        std::string network = clientMessage->update_server_message().network_name();
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Network: " << network;
#endif
        if(!DBConnector_->networkExists(network)) {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Network: " << network << " does not exist";
#endif
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
        }

        if(!DBConnector_->canAddServer(login, network)) {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "User: " << login << " cannot add server to network: " << network;
#endif
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
        }
        int port = clientMessage->update_server_message().port_number();

        std::string ip = address_.to_string();

        db::DB_Response db_response = DBConnector_->updateServer(network, login, ip, port,
                                                                 utils::currTimeInMS() + consts::SERVER_TTL);

        switch(db_response) {
        case db::INTERNAL_ERROR:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        case db::NOT_FOUND:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
        case db::NOT_PERMITED:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
        case db::NOT_UNIQUE:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
        case db::OK:
            return NodeMessageFactory::updateServerResponse(consts::SERVER_TTL);
        default:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        }

    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleStop_server(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_stop_server_message()) {
        std::string cookie = clientMessage->stop_server_message().cookie();
        std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
        if(login == "")
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

        std::string network = clientMessage->stop_server_message().network_name();
        if(!DBConnector_->networkExists(network)) {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Network: " << network << " does not exist";
#endif
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_NETWORK);
        }

        db::DB_Response db_response = DBConnector_->deleteServer(network, login);
        switch(db_response) {
        case db::INTERNAL_ERROR:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        case db::NOT_FOUND:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
        case db::NOT_PERMITED:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
        case db::NOT_UNIQUE:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
        case db::OK:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
        default:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        }

    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleSign_key(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_sign_key_message()) {

    }
    return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
}

NodeMessage_ptr RequestHandler::handleChange_password(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_change_password_message()) {
        std::string cookie = clientMessage->change_password_message().cookie();
        std::string login = sessionControler->getLogin(cookie);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Cookie: " << cookie << " login: " << ((login=="")?"NOT_LOGGED":login);
#endif
        if(login == "")
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

        std::string oldPass = clientMessage->change_password_message().old_password();
        std::string newPass = clientMessage->change_password_message().new_password();
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Old password: " << oldPass << ", new password: " << newPass;
#endif
        if(!goodCredentials(login, oldPass)) {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "User: " << login << " with password: " << oldPass << " does not exist";
#endif
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::BAD_CREDENTIALS);
        }

        std::string hash = utils::getPasswordHash(newPass);
        db::DB_Response db_response = DBConnector_->changeUserPassword(login, hash);
        switch(db_response) {
        case db::INTERNAL_ERROR:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        case db::NOT_FOUND:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_SUCH_USER);
        case db::NOT_PERMITED:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
        case db::NOT_UNIQUE:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::ALREADY_EXISTS);
        case db::OK:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
        default:
            return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}


bool RequestHandler::goodCredentials(const std::string &login, const std::string &password) const {
    std::string hash = DBConnector_->getPassHash(login);
    return Botan::check_bcrypt(password, hash);
}

} // namespace tracker
} // namespace sp2p
