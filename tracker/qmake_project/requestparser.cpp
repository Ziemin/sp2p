#include "requestparser.hpp"

#include <iostream>
#include <ostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/logic/tribool.hpp>

namespace sp2p {
namespace tracker {


RequestParser::RequestParser()
{
    this->len = -1;
}

RequestParser::RequestParser(std::int32_t len) :
    len(len)
{
}

boost::tribool RequestParser::parse(protocol_factory::AbstractRequest *clientMessage, const char data[], int length)
{
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Parsing message";
#endif
    boost::asio::streambuf streambuf_;
    std::ostream os(&streambuf_);
    os.write(data, length);

    if(len == -1 && streambuf_.size() < sizeof(std::int32_t)) {
        return boost::indeterminate;
    }

    if(len == -1) {
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "buff size before: " << streambuf_.size();
#endif
        std::istream istream(&streambuf_);
        istream.read(reinterpret_cast<char*>(&len), sizeof(len));

#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "buff size after: " << streambuf_.size();
#endif
    }

    if(streambuf_.size() < len) {
        return boost::indeterminate;
    }

    if(streambuf_.size() == len) {
        std::istream istream(&streambuf_);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "before parsing";
#endif
        bool ret = clientMessage->parseFromIstream(&istream);
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "after parsing";
#endif
        return ret;
    }

    return false;


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

