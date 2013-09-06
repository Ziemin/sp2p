#include "response.hpp"

#include <iostream>

namespace sp2p {
namespace tracker {


Response::Response()
{
    message = new protocol::NodeMessage();
}


bool sp2p::tracker::Response::parseIntoOstream(std::ostream *stream)
{
    std::int32_t messageSize = getSize();
    stream->write(reinterpret_cast<const char*>(&messageSize), sizeof(messageSize));
    return this->message->SerializePartialToOstream(stream);
}

int sp2p::tracker::Response::getSize() const
{
    return message->ByteSize();
}

protocol::NodeMessage *Response::getMessage() const
{
    return message;
}

void Response::setMessage(protocol::NodeMessage *value)
{
    message = value;
}


} // namespace tracker
} // namespace sp2p
