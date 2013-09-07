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

namespace sp2p {
namespace tracker {

RequestHandler::RequestHandler(SessionControler_ptr sessionControler, DBConnector_ptr DBConnector_)
{
    this->sessionControler = sessionControler;
    this->DBConnector_ = DBConnector_;
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

bool RequestHandler::cookieCorrect(const protocol::ClientMessage *clientMessage) const {
}


NodeMessage_ptr RequestHandler::handleRegister(const protocol::ClientMessage *clientMessage) const {

    // TO DO

    if(clientMessage->has_register_message()) {
        if(clientMessage->register_message().has_username()
                && clientMessage->register_message().has_password()
                && clientMessage->register_message().has_public_key()) {

            std::string login = clientMessage->register_message().username();
            std::string pass = clientMessage->register_message().password();
            db::DB_Response db_response = DBConnector_->createUser(login, pass);
            switch(db_response) {
            case db::NOT_UNIQUE:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::BAD_DATA);
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            default:
                std::string cookie = sessionControler->login(login);
                std::string cert = "notimplementedyet";
                return NodeMessageFactory::loginResponse(cookie, cert);
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
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

            db::DB_Response db_response = DBConnector_->removeUser(login);
            switch(db_response) {
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::BAD_DATA);
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
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
            bool db_response = DBConnector_->isUser(login, pass);
            if(!db_response)
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
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            auto L = DBConnector_->getAllNetworks(login);
            if(L == nullptr)
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            return NodeMessageFactory::listNetworksResponse(L);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleList_my_networks(const protocol::ClientMessage *clientMessage) const {
    if(clientMessage->has_list_networks_message()) {
        if(clientMessage->list_networks_message().has_cookie()) {
            std::string cookie = clientMessage->list_networks_message().cookie();
            std::string login = sessionControler->getLogin(cookie);
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            auto L = DBConnector_->getUserNetworks(login);
            if(L == nullptr)
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            return NodeMessageFactory::listNetworksResponse(L);
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleList_servers(const protocol::ClientMessage *clientMessage) const {
    // TO DO
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
            std::string login = sessionControler->login(cookie);

            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);

            std::string name = clientMessage->create_network_message().network_name();
            bool accessRights = (clientMessage->create_network_message().access_rights()
                                 == protocol::ClientMessage::CreateNetwork::PUBLIC);
            bool visible = (clientMessage->create_network_message().visibility()
                            == protocol::ClientMessage::CreateNetwork::LISTED);
            bool participable = (clientMessage->create_network_message().participation_rights()
                                 == protocol::ClientMessage::CreateNetwork::CLIENT_SERVER);
            std::string protocol = "";
            if(clientMessage->create_network_message().has_protocol_name())
                protocol = clientMessage->create_network_message().protocol_name();
            std::string ownerEmail = "";
            if(clientMessage->create_network_message().has_creator_email())
                ownerEmail = clientMessage->create_network_message().creator_email();

            db::DB_Response db_response = DBConnector_->createNetwork(name, login, accessRights,
                                                                      visible, participable, ownerEmail,
                                                                      protocol);
            switch(db_response) {
            case db::NOT_FOUND:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::BAD_DATA);
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_UNIQUE:

                // TO DO
                // ALREADY_EXISTS
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::BAD_REQUEST);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
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
            std::string login = sessionControler->login(cookie);
            if(login == "")
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NOT_LOGGED);
            std::string name = clientMessage->delete_network_message().name();
            db::DB_Response db_response = DBConnector_->deleteNetwork(login, name);
            switch(db_response) {
            case db::INTERNAL_ERROR:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::INTERNAL_SERVER_ERROR);
            case db::NOT_PERMITED:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::NO_PRIVILAGES);
            default:
                return NodeMessageFactory::errorMesage(protocol::NodeMessage::OK);
            }
        }
    }
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleInvite_user(const protocol::ClientMessage *clientMessage) const {
    // - -
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleRemove_user(const protocol::ClientMessage *clientMessage) const {
    // - -
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleUser_info(const protocol::ClientMessage *clientMessage) const {
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleUpdate_server(const protocol::ClientMessage *clientMessage) const {
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleStop_server(const protocol::ClientMessage *clientMessage) const {
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleSign_key(const protocol::ClientMessage *clientMessage) const {
    return NodeMessageFactory::badRequestMessage();
}

NodeMessage_ptr RequestHandler::handleChange_password(const protocol::ClientMessage *clientMessage) const {
    // - -
    return NodeMessageFactory::badRequestMessage();
}


} // namespace tracker
} // namespace sp2p
