#include "requesthandler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "proto/sp2p_protocol.pb.h"
#include "request.hpp"
#include "response.hpp"

namespace sp2p {
namespace tracker {

RequestHandler::RequestHandler(SessionControler_ptr sessionControler)
{
    this->sessionControler = sessionControler;
}

void RequestHandler::handleRequest(const protocol_factory::AbstractRequest *req, protocol_factory::AbstractResponse *rep) const
{
    Request *request = (Request*) req;
    Response *response = (Response*) rep;

    protocol::ClientMessage *clientMessage = request->getMessage();
    protocol::NodeMessage *nodeMessage = response->getMessage();

    switch(clientMessage->request()) {
    case protocol::ClientMessage::REGISTER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "REGISTER";
#endif
        break;
    case protocol::ClientMessage::UNREGISTER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "UNREGISTER";
#endif
        break;
    case protocol::ClientMessage::LOGIN:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LOGIN";
#endif
        break;
    case protocol::ClientMessage::LOGOUT:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LOGOUT";
#endif
        break;
    case protocol::ClientMessage::LIST_NETWORKS:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LIST_NETWORKS";
#endif
        break;
    case protocol::ClientMessage::LIST_MY_NETWORKS:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LIST_MY_NETWORKS";
#endif
        break;
    case protocol::ClientMessage::LIST_SERVERS:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "LIST_SERVERS";
#endif
        break;
    case protocol::ClientMessage::CREATE_NETWORK:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "CREATE_NETWORK";
#endif
        break;
    case protocol::ClientMessage::DELETE_NETWORK:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "DELETE_NETWORK";
#endif
        break;
    case protocol::ClientMessage::INVITE_USER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "INVITE_USER";
#endif
        break;
    case protocol::ClientMessage::REMOVE_USER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "REMOVE_USER";
#endif
        break;
    case protocol::ClientMessage::USER_INFO:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "USER_INFO";
#endif
        break;
    case protocol::ClientMessage::UPDATE_SERVER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "UPDATE_SERVER";
#endif
        break;
    case protocol::ClientMessage::STOP_SERVER:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "STOP_SERVER";
#endif
        break;
    case protocol::ClientMessage::SIGN_KEY:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "SIGN_KEY";
#endif
        break;
    case protocol::ClientMessage::CHANGE_PASSWORD:
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "CHANGE_PASSWORD";
#endif
        break;
    default:
        // bad request
        break;
    }
}

void RequestHandler::badRequestResponse(protocol_factory::AbstractResponse *rep) const {

}

bool RequestHandler::cookieCorrect(protocol::ClientMessage *clientMessage) {

}


void RequestHandler::handleRegister(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleUnregister(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleLogin(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleLogout(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleList_networks(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleList_my_networks(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleList_servers(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleCreate_network(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleDelete_network(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleInvite_user(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleRemove_user(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleUser_info(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleUpdate_server(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleStop_server(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleSign_key(protocol::ClientMessage *clientMessage) {

}

void RequestHandler::handleChange_password(protocol::ClientMessage *clientMessage) {

}


} // namespace tracker
} // namespace sp2p
