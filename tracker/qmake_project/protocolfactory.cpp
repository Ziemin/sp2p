#include "protocolfactory.hpp"

#include "types.hpp"
#include "request.hpp"
#include "response.hpp"
#include "requesthandler.hpp"
#include "requestparser.hpp"

namespace sp2p {
namespace tracker {

ProtocolFactory::ProtocolFactory(SessionControler_ptr sessionControler) :
    AbstractProtocolFactory()
{
    this->sessionControler = sessionControler;
}


protocol_factory::AbstractRequest *ProtocolFactory::produceRequest() const
{
    return new Request();
}

protocol_factory::AbstractRequestHandler *ProtocolFactory::produceRequestHandler() const
{
    return new RequestHandler(sessionControler);
}

protocol_factory::AbstractRequestParser *ProtocolFactory::produceRequestParser() const
{
    return new RequestParser();
}

protocol_factory::AbstractResponse *ProtocolFactory::produceResponse() const
{
    return new Response();
}

SessionControler_ptr ProtocolFactory::getSessionControler() const
{
    return sessionControler;
}

void ProtocolFactory::setSessionControler(const SessionControler_ptr &value)
{
    sessionControler = value;
}



} // namespace tracker
} // namespace sp2p
