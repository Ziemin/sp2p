#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include "requesthandler.hpp"
#include "utils/util_functions.h"

namespace sp2p {
namespace tracker {

Connection::Connection(boost::asio::io_service& io_service,
                       Factory_ptr factory)
    : strand_(io_service),
      socket_(io_service)
{
    this->factory = factory;
    this->requestParser = RequestParser_ptr(factory->produceRequestParser());
    this->request_ = Request_ptr(factory->produceRequest());
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return socket_;
}

void Connection::start()
{
#ifdef DEBUG_LOGGING
    BOOST_LOG_TRIVIAL(debug) << "New connection";
#endif
    this->requestHandler = RequestHandler_ptr(factory->produceRequestHandler(socket_.remote_endpoint().address()));
    socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
                                boost::bind(&Connection::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred)));
}


void Connection::handle_read(const boost::system::error_code& e,
                             std::size_t bytes_transferred)
{
#ifdef DEBUG_LOGGING
    BOOST_LOG_TRIVIAL(debug) << "Reading body";
    BOOST_LOG_TRIVIAL(debug) << "Bytes transfered: " << bytes_transferred;
#endif
    if (!e)
    {
        boost::tribool result;
        result = requestParser->parse(request_.get(), buffer_.data(), bytes_transferred);

        if (result)
        {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "WTF!? Resault, dude!";
#endif
            reply_ = Response_ptr(factory->produceResponse());
            requestHandler->handleRequest(request_.get(), reply_.get());
            std::ostream ostream(&outBuff);
            reply_->parseIntoOstream(&ostream);
            boost::asio::async_write(socket_, outBuff,
                                     strand_.wrap(
                                         boost::bind(&Connection::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error)));
        }
        else if (!result)
        {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Damn you, resault! Next time you'll be mine!";
#endif
            reply_ = Response_ptr(factory->produceResponse());
            requestHandler->badRequestResponse(reply_.get());
            std::ostream ostream(&outBuff);
            reply_->parseIntoOstream(&ostream);
            boost::asio::async_write(socket_, outBuff,
                                     strand_.wrap(
                                         boost::bind(&Connection::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error)));
        }
        else
        {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Yes? No? Pomidor.";
#endif
            socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
                                        boost::bind(&Connection::handle_read, shared_from_this(),
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred)));
        }
    }

}

void Connection::handle_write(const boost::system::error_code& e)
{
    if (!e)
    {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }

}

} // namespace tracker
} // namespace sp2p
