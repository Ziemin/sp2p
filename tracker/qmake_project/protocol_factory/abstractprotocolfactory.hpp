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

/**
 * @brief The AbstractProtocolFactory class - abstract factory producing all
 * protocol components - Request, Response, RequestHandler and RequestParser
 */
class AbstractProtocolFactory : public std::enable_shared_from_this<AbstractProtocolFactory>
{
public:
    /**
     * @brief produceRequest
     * @return pointer to new object inheriting AbstractRequest
     */
    virtual AbstractRequest *produceRequest() const = 0;

    /**
     * @brief produceRequestHandler
     * @return  pointer to new object inheriting AbstractRequestHandler
     */
    virtual AbstractRequestHandler *produceRequestHandler(boost::asio::ip::address) const = 0;

    /**
     * @brief produceRequestParser
     * @return  pointer to new object inheriting AbstractRequestParser
     */
    virtual AbstractRequestParser *produceRequestParser() const = 0;

    /**
     * @brief produceResponse
     * @return  pointer to new object inheriting AbstractResponse
     */
    virtual AbstractResponse *produceResponse() const = 0;
    virtual ~AbstractProtocolFactory() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTPROTOCOLFACTORY_HPP
