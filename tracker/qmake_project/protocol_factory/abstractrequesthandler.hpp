#ifndef ABSTRACTREQUESTHANDLER_HPP
#define ABSTRACTREQUESTHANDLER_HPP

#include "abstractrequest.hpp"
#include "abstractresponse.hpp"
#include <boost/noncopyable.hpp>

namespace sp2p {
namespace tracker {
namespace protocol_factory {

class AbstractRequestHandler
        : private boost::noncopyable
{
public:
    virtual void handleRequest(const AbstractRequest *req, AbstractResponse *rep) const = 0;
    virtual ~AbstractRequestHandler() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTREQUESTHANDLER_HPP
