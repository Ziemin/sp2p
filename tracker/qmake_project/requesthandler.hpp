#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>
#include "types.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractresponse.hpp"

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

private:

};

} // namespace tracker
} // namespace sp2p
#endif // REQUESTHANDLER_H
