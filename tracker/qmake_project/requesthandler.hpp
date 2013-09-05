#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>
#include "types.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractresponse.hpp"
#include "request.hpp"
#include "response.hpp"

namespace sp2p {
namespace tracker {

/// The common handler for all incoming requests.
class RequestHandler
        : public protocol_factory::AbstractRequestHandler
{
public:
    /// Construct with a directory containing files to be served.
    explicit RequestHandler();

    /// Handle a request and produce a reply.
    void handleRequest(const protocol_factory::AbstractRequest *req, protocol_factory::AbstractResponse *rep) const;
    void badRequestResponse(protocol_factory::AbstractResponse *rep) const;

private:
    bool cookieCorrect(Request *req);

    void handleRegister(Request *req);
    void handleUnregister(Request *req);
    void handleLogin(Request *req);
    void handleLogout(Request *req);
    void handleList_networks(Request *req);
    void handleList_my_networks(Request *req);
    void handleList_servers(Request *req);
    void handleCreate_network(Request *req);
    void handleDelete_network(Request *req);
    void handleInvite_user(Request *req);
    void handleRemove_user(Request *req);
    void handleUser_info(Request *req);
    void handleUpdate_server(Request *req);
    void handleStop_server(Request *req);
    void handleSign_key(Request *req);
    void handleChange_password(Request *req);


};

} // namespace tracker
} // namespace sp2p
#endif // REQUESTHANDLER_H
