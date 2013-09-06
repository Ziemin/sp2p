#ifndef TYPES_HPP
#define TYPES_HPP

#include <memory>
#include "proto/sp2p_protocol.pb.h"
#include "protocol_factory/abstractprotocolfactory.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractrequestparser.hpp"
#include "protocol_factory/abstractresponse.hpp"
#include "protocol_factory/abstractsessioncontroler.hpp"
#include "db/abstractdbconnector.h"


typedef std::shared_ptr<protocol::ClientMessage> ClientMessage_ptr;
typedef std::shared_ptr<protocol::NodeMessage> NodeMessage_ptr;
typedef std::shared_ptr<sp2p::tracker::protocol_factory::AbstractRequest> Request_ptr;
typedef std::shared_ptr<sp2p::tracker::protocol_factory::AbstractResponse> Response_ptr;
typedef std::shared_ptr<sp2p::tracker::protocol_factory::AbstractRequestParser> RequestParser_ptr;
typedef std::shared_ptr<sp2p::tracker::protocol_factory::AbstractRequestHandler> RequestHandler_ptr;
typedef std::shared_ptr<sp2p::tracker::protocol_factory::AbstractProtocolFactory> Factory_ptr;
typedef std::shared_ptr<sp2p::tracker::protocol_factory::AbstractSessionControler> SessionControler_ptr;
typedef std::shared_ptr<sp2p::tracker::db::AbstractDBConnector> DBConnector_ptr;


#endif // TYPES_HPP
