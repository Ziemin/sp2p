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

RequestHandler::RequestHandler()
{
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

bool RequestHandler::cookieCorrect(Request *req) {

}


void RequestHandler::handleRegister(Request *req) {

}

void RequestHandler::handleUnregister(Request *req) {

}

void RequestHandler::handleLogin(Request *req) {

}

void RequestHandler::handleLogout(Request *req) {

}

void RequestHandler::handleList_networks(Request *req) {

}

void RequestHandler::handleList_my_networks(Request *req) {

}

void RequestHandler::handleList_servers(Request *req) {

}

void RequestHandler::handleCreate_network(Request *req) {

}

void RequestHandler::handleDelete_network(Request *req) {

}

void RequestHandler::handleInvite_user(Request *req) {

}

void RequestHandler::handleRemove_user(Request *req) {

}

void RequestHandler::handleUser_info(Request *req) {

}

void RequestHandler::handleUpdate_server(Request *req) {

}

void RequestHandler::handleStop_server(Request *req) {

}

void RequestHandler::handleSign_key(Request *req) {

}

void RequestHandler::handleChange_password(Request *req) {

}


} // namespace tracker
} // namespace sp2p
