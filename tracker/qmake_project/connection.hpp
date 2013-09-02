#ifndef SP2PCONNECTION_H
#define SP2PCONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
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

typedef boost::shared_ptr<protocol_factory::AbstractRequest> Request_ptr;
typedef boost::shared_ptr<protocol_factory::AbstractResponse> Response_ptr;

/// Represents a single connection from a client.
class Connection
  : public boost::enable_shared_from_this<Connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit Connection(boost::asio::io_service& io_service,
      const protocol_factory::AbstractRequestHandler *handler);

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();

private:
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);

  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_service::strand strand_;

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The handler used to process the incoming request.
  const protocol_factory::AbstractRequestHandler* requestHandler;

  /// Buffer for incoming data.
  boost::array<char, 8192> buffer_;

  /// The incoming request.
  Request_ptr request_;

  /// The parser for the incoming request.
  protocol_factory::AbstractRequestParser* requestParser;

  /// The reply to be sent back to the client.
  Response_ptr reply_;

  boost::asio::streambuf buff;
};


typedef boost::shared_ptr<Connection> Connection_ptr;

} // namespace tracker
} // namespace sp2p
#endif // SP2PCONNECTION_H
