#include "request.hpp"

#include <boost/log/trivial.hpp>

namespace sp2p {
namespace tracker {

Request::Request()
{
    message = new protocol::ClientMessage();
}

bool Request::parseFromIstream(std::istream *stream) {
    return message->ParseFromIstream(stream);
}

int Request::getSize() const {
    return message->ByteSize();
}

void Request::setMessage(protocol::ClientMessage *value)
{
    message = value;
}


protocol::ClientMessage *Request::getMessage() const
{
    return message;
}


} // namespace tracker
} // namespace sp2p
