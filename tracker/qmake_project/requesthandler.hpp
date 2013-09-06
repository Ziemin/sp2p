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
    RequestHandler(SessionControler_ptr sessionControler);

    /// Handle a request and produce a reply.
    void handleRequest(const protocol_factory::AbstractRequest *req, protocol_factory::AbstractResponse *rep) const;
    void badRequestResponse(protocol_factory::AbstractResponse *rep) const;

private:
    bool cookieCorrect(protocol::ClientMessage *clientMessage);

    void handleRegister(protocol::ClientMessage *clientMessage);
    void handleUnregister(protocol::ClientMessage *clientMessage);
    void handleLogin(protocol::ClientMessage *clientMessage);
    void handleLogout(protocol::ClientMessage *clientMessage);
    void handleList_networks(protocol::ClientMessage *clientMessage);
    void handleList_my_networks(protocol::ClientMessage *clientMessage);
    void handleList_servers(protocol::ClientMessage *clientMessage);
    void handleCreate_network(protocol::ClientMessage *clientMessage);
    void handleDelete_network(protocol::ClientMessage *clientMessage);
    void handleInvite_user(protocol::ClientMessage *clientMessage);
    void handleRemove_user(protocol::ClientMessage *clientMessage);
    void handleUser_info(protocol::ClientMessage *clientMessage);
    void handleUpdate_server(protocol::ClientMessage *clientMessage);
    void handleStop_server(protocol::ClientMessage *clientMessage);
    void handleSign_key(protocol::ClientMessage *clientMessage);
    void handleChange_password(protocol::ClientMessage *clientMessage);

    SessionControler_ptr sessionControler;

};

} // namespace tracker
} // namespace sp2p
#endif // REQUESTHANDLER_H
