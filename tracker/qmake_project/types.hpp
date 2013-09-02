#ifndef TYPES_HPP
#define TYPES_HPP

#include <memory>
#include "proto/sp2p_protocol.pb.h"

typedef std::shared_ptr<protocol::ClientMessage> ClientMessage_ptr;
typedef std::shared_ptr<protocol::NodeMessage> NodeMessage_ptr;

#endif // TYPES_HPP
