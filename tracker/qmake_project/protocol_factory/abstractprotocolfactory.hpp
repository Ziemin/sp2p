#ifndef ABSTRACTPROTOCOLFACTORY_HPP
#define ABSTRACTPROTOCOLFACTORY_HPP

#include <memory>
#include <boost/asio.hpp>

#include "abstractrequest.hpp"
#include "abstractrequesthandler.hpp"
#include "abstractrequestparser.hpp"
#include "abstractresponse.hpp"


namespace sp2p {
namespace tracker {
namespace protocol_factory {

class AbstractProtocolFactory : public std::enable_shared_from_this<AbstractProtocolFactory>
{
public:
    virtual AbstractRequest *produceRequest() const = 0;
    virtual AbstractRequestHandler *produceRequestHandler(boost::asio::ip::tcp::endpoint) const = 0;
    virtual AbstractRequestParser *produceRequestParser() const = 0;
    virtual AbstractResponse *produceResponse() const = 0;
    virtual ~AbstractProtocolFactory() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTPROTOCOLFACTORY_HPP
