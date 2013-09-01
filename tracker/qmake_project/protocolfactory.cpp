#include "protocolfactory.hpp"

#include "request.hpp"
#include "response.hpp"
#include "requesthandler.hpp"
#include "requestparser.hpp"

namespace sp2p {
namespace tracker {

ProtocolFactory::ProtocolFactory()
{
}


sp2p::tracker::protocol_factory::AbstractRequest *ProtocolFactory::produceRequest()
{
    return new Request();
}

protocol_factory::AbstractRequestHandler *ProtocolFactory::produceRequestHandler()
{
    return new RequestHandler();
}

protocol_factory::AbstractRequestParser *ProtocolFactory::produceRequestParser()
{
    return new RequestParser();
}

protocol_factory::AbstractResponse *ProtocolFactory::produceResponse()
{
    return new Response();
}


} // namespace tracker
} // namespace sp2p
