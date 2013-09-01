#include "requestparser.hpp"

#include <iostream>
#include <ostream>

namespace sp2p {
namespace tracker {


sp2p::tracker::RequestParser::RequestParser()
{
    this->len = -1;
}

sp2p::tracker::RequestParser::RequestParser(int len) :
    len(len)
{
}

boost::tribool sp2p::tracker::RequestParser::parse(protocol_factory::AbstractRequest *clientMessage, const char data[], int length)
{
//    std::ostream os;
//    os.write(data, len);

//    clientMessage->
}

int RequestParser::getLen() const
{
    return len;
}

void RequestParser::setLen(int value)
{
    len = value;
}



} // namespace tracker
} // namespace sp2p

