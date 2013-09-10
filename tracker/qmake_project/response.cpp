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

std::string Response::status()
{
    switch(message->response_type()){
    case protocol::NodeMessage_ResponseType_INTERNAL_SERVER_ERROR:
        return "INTERNAL_SERVER_ERROR";
    case protocol::NodeMessage_ResponseType_BAD_REQUEST:
        return "BAD_REQUEST";
    case protocol::NodeMessage_ResponseType_NO_PRIVILAGES:
        return "NO_PRIVILAGES";
    case protocol::NodeMessage_ResponseType_NOT_LOGGED:
        return "NOT_LOGGED";
    case protocol::NodeMessage_ResponseType_BAD_CREDENTIALS:
        return "BAD_CREDENTIALS";
    case protocol::NodeMessage_ResponseType_NO_SUCH_USER:
        return "NO_SUCH_USER";
    case protocol::NodeMessage_ResponseType_NO_SUCH_NETWORK:
        return "NO_SUCH_NETWORK";
    case protocol::NodeMessage_ResponseType_BAD_DATA:
        return "BAD_DATA";
    case protocol::NodeMessage_ResponseType_ALREADY_EXISTS:
        return "ALREADY_EXISTS";
    default:
        return "OK";
    }

}


} // namespace tracker
} // namespace sp2p
