#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "proto/sp2p_protocol.pb.h"

#include "protocol_factory/abstractrequest.hpp"


namespace sp2p {
namespace tracker {

/**
 * @brief The Request class - implementation od AbstractRequest for sp2p protocl
 */
class Request : public protocol_factory::AbstractRequest
{
public:
    Request();
    bool parseFromIstream(std::istream *stream);
    int getSize() const;
    protocol::ClientMessage *getMessage();
    void setMessage(protocol::ClientMessage *value);

private:
    protocol::ClientMessage *message;
};

} // namespace tracker
} // namespace sp2p

#endif // REQUEST_HPP
