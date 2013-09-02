#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "requesthandler.hpp"

namespace sp2p {
namespace tracker {

Connection::Connection(boost::asio::io_service& io_service,
                       const protocol_factory::AbstractRequestHandler *handler)
    : strand_(io_service),
      socket_(io_service)
{
    this->requestHandler = handler;
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return socket_;
}

void Connection::start()
{
    socket_.async_read_some(boost::asio::buffer(buffer_),
                            strand_.wrap(
                                boost::bind(&Connection::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred)));
}

void Connection::handle_read(const boost::system::error_code& e,
                             std::size_t bytes_transferred)
{
    if (!e)
    {
        boost::tribool result;
        result = requestParser->parse(request_.get(), buffer_.data(), bytes_transferred);

        if (result)
        {
            requestHandler->handleRequest(request_.get(), reply_.get());
            std::ostream ostream(&this->buff);
            reply_->parseIntoOstream(&ostream);
            boost::asio::async_write(socket_, this->buff,
                                     strand_.wrap(
                                         boost::bind(&Connection::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error)));
        }
        else if (!result)
        {
            // reply with BAD_REQUEST
            //      reply_ = reply::stock_reply(reply::bad_request);
            //      boost::asio::async_write(socket_, reply_.to_buffers(),
            //          strand_.wrap(
            //            boost::bind(&Connection::handle_write, shared_from_this(),
            //              boost::asio::placeholders::error)));
        }
        else
        {
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    strand_.wrap(
                                        boost::bind(&Connection::handle_read, shared_from_this(),
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred)));
        }
    }

    // If an error occurs then no new asynchronous operations are started. This
    // means that all shared_ptr references to the connection object will
    // disappear and the object will be destroyed automatically after this
    // handler returns. The connection class's destructor closes the socket.
}

void Connection::handle_write(const boost::system::error_code& e)
{
    if (!e)
    {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }

    // No new asynchronous operations are started. This means that all shared_ptr
    // references to the connection object will disappear and the object will be
    // destroyed automatically after this handler returns. The connection class's
    // destructor closes the socket.
}

} // namespace tracker
} // namespace sp2p
