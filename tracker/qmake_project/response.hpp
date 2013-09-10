#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <boost/asio/buffer.hpp>
#include <string>

#include "protocol_factory/abstractresponse.hpp"
#include "proto/sp2p_protocol.pb.h"

namespace sp2p {
namespace tracker {

class Response : public protocol_factory::AbstractResponse
{
public:
    Response();
    bool parseIntoOstream(std::ostream *stream);
    int getSize() const;
    protocol::NodeMessage *getMessage() const;
    void setMessage(protocol::NodeMessage *value);
    std::string status();

private:
    protocol::NodeMessage *message;
};

} // namespace tracker
} // namespace sp2p

#endif // RESPONSE_HPP
