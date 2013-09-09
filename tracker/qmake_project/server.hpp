#ifndef SP2PSERVER_H
#define SP2PSERVER_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "types.hpp"
#include "connection.hpp"
#include "requesthandler.hpp"
#include "protocol_factory/abstractprotocolfactory.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractrequesthandler.hpp"
#include "protocol_factory/abstractrequestparser.hpp"
#include "protocol_factory/abstractresponse.hpp"

namespace sp2p {
namespace tracker {

/// The top-level class of the server.
class Server
        : private boost::noncopyable
{
public:
    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit Server(Factory_ptr factory, const std::string& address, const std::string& port, std::size_t thread_pool_size);

    /// Run the server's io_service loop.
    void run();

private:
    /// Initiate an asynchronous accept operation.
    void start_accept();

    /// Handle completion of an asynchronous accept operation.
    void handle_accept(const boost::system::error_code& e);

    std::size_t thread_pool_size_;
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    Connection_ptr new_connection_;
//    protocol_factory::AbstractRequestHandler *requestHandler;
    Factory_ptr protocolFactory;
};

} // namespace tracker
} // namespace sp2p
#endif // SP2PSERVER_H
