#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <memory>
#include "types.hpp"
#include "request.hpp"
#include "response.hpp"
#include <boost/noncopyable.hpp>
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractresponse.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractsessioncontroler.hpp"

namespace sp2p {
namespace tracker {

/// The common handler for all incoming requests.
class RequestHandler
        : public protocol_factory::AbstractRequestHandler
{
public:
    /// Construct with a directory containing files to be served.
    RequestHandler(SessionControler_ptr sessionControler, DBConnector_ptr DBConnector_);

    /// Handle a request and produce a reply.
    void handleRequest(const protocol_factory::AbstractRequest *req, protocol_factory::AbstractResponse *rep) const;
    void badRequestResponse(protocol_factory::AbstractResponse *rep) const;

private:
    bool cookieCorrect(const protocol::ClientMessage *clientMessage) const;

    NodeMessage_ptr handleRegister(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleUnregister(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleLogin(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleLogout(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleList_networks(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleList_my_networks(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleList_servers(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleCreate_network(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleDelete_network(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleInvite_user(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleRemove_user(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleUser_info(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleUpdate_server(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleStop_server(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleSign_key(const protocol::ClientMessage *clientMessage) const;
    NodeMessage_ptr handleChange_password(const protocol::ClientMessage *clientMessage) const;

    SessionControler_ptr sessionControler;
    DBConnector_ptr DBConnector_;
};

} // namespace tracker
} // namespace sp2p
#endif // REQUESTHANDLER_H
