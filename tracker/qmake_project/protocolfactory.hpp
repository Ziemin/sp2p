#ifndef PROTOCOLFACTORY_HPP
#define PROTOCOLFACTORY_HPP

#include "protocol_factory/abstractprotocolfactory.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractrequestparser.hpp"
#include "protocol_factory/abstractresponse.hpp"

namespace sp2p {
namespace tracker {

class ProtocolFactory : public protocol_factory::AbstractProtocolFactory
{
public:
    ProtocolFactory();
    protocol_factory::AbstractRequest *produceRequest()const;
    protocol_factory::AbstractRequestHandler *produceRequestHandler() const;
    protocol_factory::AbstractRequestParser *produceRequestParser() const;
    protocol_factory::AbstractResponse *produceResponse() const;
};

} // namespace tracker
} // namespace sp2p

#endif // PROTOCOLFACTORY_HPP
