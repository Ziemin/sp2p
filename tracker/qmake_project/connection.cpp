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
                       boost::asio::ssl::context& context,
                       Factory_ptr factory)
    : strand_(io_service),
      socket_(io_service, context)
{
    this->factory = factory;
    this->requestParser = RequestParser_ptr(factory->produceRequestParser());
    this->request_ = Request_ptr(factory->produceRequest());
}

boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type& Connection::socket()
{
  return socket_.lowest_layer();
}

void Connection::start()
{
#ifdef DEBUG_LOGGING
    BOOST_LOG_TRIVIAL(debug) << "New connection from " << socket().remote_endpoint().address().to_string();
#endif
    this->requestHandler = RequestHandler_ptr(factory->produceRequestHandler(socket().remote_endpoint().address()));
    socket_.async_handshake(boost::asio::ssl::stream_base::server,
                                boost::bind(&Connection::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error, 0));
    //    socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
    //                                boost::bind(&Connection::handle_read, shared_from_this(),
    //                                            boost::asio::placeholders::error,
    //                                            boost::asio::placeholders::bytes_transferred)));
}


void Connection::handle_handshake(const boost::system::error_code& error)
{
    if (!error)
    {
#ifdef DEBUG_LOGGING
    BOOST_LOG_TRIVIAL(debug) << "Handshake complete";
#endif
        socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
                                    boost::bind(&Connection::handle_read, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred)));
    }
    else
    {
#ifdef DEBUG_LOGGING
    BOOST_LOG_TRIVIAL(debug) << "Handshake failed. Error: " << error.message();
#endif
    }
}


void Connection::handle_read(const boost::system::error_code& e,
                             std::size_t bytes_transferred)
{
    if (!e)
    {
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "No i teges...";
#endif

        boost::tribool result;
        result = requestParser->parse(request_.get(), buffer_.data(), bytes_transferred);

        if (result)
        {
            reply_ = Response_ptr(factory->produceResponse());
            requestHandler->handleRequest(request_.get(), reply_.get());
            std::ostream ostream(&outBuff);
            reply_->parseIntoOstream(&ostream);
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Sending reply to: " << socket().remote_endpoint().address().to_string()
                                     << " size: " << reply_->getSize() << " status: " << reply_->status();
#endif
            boost::asio::async_write(socket_, outBuff,
                                     strand_.wrap(
                                         boost::bind(&Connection::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error)));
        }
        else if (!result)
        {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Result not parsed";
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
            socket_.async_read_some(boost::asio::buffer(buffer_), strand_.wrap(
                                        boost::bind(&Connection::handle_read, shared_from_this(),
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred)));
        }
    }
    else {
#ifdef DEBUG_LOGGING
            BOOST_LOG_TRIVIAL(debug) << "Error in handle_read: " << e.message();
#endif
    }
}

void Connection::handle_write(const boost::system::error_code& e)
{
    if (!e)
    {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }

}

} // namespace tracker
} // namespace sp2p
