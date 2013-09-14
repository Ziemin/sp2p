#ifndef SP2PCONNECTION_H
#define SP2PCONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include <boost/enable_shared_from_this.hpp>
#include "requesthandler.hpp"
#include "requestparser.hpp"
#include "types.hpp"

#include "protocol_factory/abstractprotocolfactory.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractrequestparser.hpp"
#include "protocol_factory/abstractresponse.hpp"

namespace sp2p {
namespace tracker {

/// Represents a single connection from a client.
class Connection
        : public boost::enable_shared_from_this<Connection>,
        private boost::noncopyable
{
public:
    /// Construct a connection with the given io_service.
    explicit Connection(boost::asio::io_service& io_service, boost::asio::ssl::context &context,
                        Factory_ptr factory);

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type& socket();


    /// Start the first asynchronous operation for the connection.
    void start();

private:

    /// Handle completion of a read operation.
    void handle_read(const boost::system::error_code& e,
                     std::size_t bytes_transferred);

    void handle_handshake(const boost::system::error_code& e);

    /// Handle completion of a write operation.
    void handle_write(const boost::system::error_code& e);

    /// Strand to ensure the connection's handlers are not called concurrently.
    boost::asio::io_service::strand strand_;

    /// Socket for the connection.
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

    Factory_ptr factory;

    /// The handler used to process the incoming request.
    RequestHandler_ptr requestHandler;

    /// Buffer for incoming data.
    boost::array<char, 8192> buffer_;

    /// The incoming request.
    Request_ptr request_;

    /// The parser for the incoming request.
    RequestParser_ptr requestParser;

    /// The reply to be sent back to the client.
    Response_ptr reply_;

    boost::asio::streambuf outBuff;

    //    boost::asio::streambuf buffer_;
};


typedef boost::shared_ptr<Connection> Connection_ptr;

} // namespace tracker
} // namespace sp2p
#endif // SP2PCONNECTION_H
