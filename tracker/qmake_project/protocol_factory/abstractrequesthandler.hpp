#ifndef ABSTRACTREQUESTHANDLER_HPP
#define ABSTRACTREQUESTHANDLER_HPP

#include "abstractrequest.hpp"
#include "abstractresponse.hpp"
#include <boost/noncopyable.hpp>

namespace sp2p {
namespace tracker {
namespace protocol_factory {


/**
 * @brief The AbstractRequestHandler class - interface for client request handler
 */
class AbstractRequestHandler
        : private boost::noncopyable
{
public:
    /**
     * @brief handleRequest - handles given request and produces response in given object
     * @param req - pointer to client request
     * @param rep - pointer to object in which response data should be stored
     */
    virtual void handleRequest(const AbstractRequest *req, AbstractResponse *rep) const = 0;

    /**
     * @brief badRequestResponse - produces response with bad request message
     * @param rep
     */
    virtual void badRequestResponse(AbstractResponse *rep) const = 0;
    virtual ~AbstractRequestHandler() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTREQUESTHANDLER_HPP
