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

/**
* @brief The Server class - top level class of the server
*/
class Server
        : private boost::noncopyable
{
public:
    /**
     * @brief Server - constructor
     * @param factory - implementation of AbstractProtofolFactory to produce protocol components
     * @param address - address to listen on
     * @param port - port to listen on
     * @param thread_pool_size - number of threads to handle connections
     */
    explicit Server(Factory_ptr factory, const std::string& address, const std::string& port, std::size_t thread_pool_size);

    /**
     * @brief run - run the server in io_service loop
     */
    void run();

private:
    /**
     * @brief start_accept - initiate an asynchronous accept operation.
     */
    void start_accept();

    /**
     * @brief handle_accept - handles completion of an asynchronous accept operation
     * @param e
     */
    void handle_accept(const boost::system::error_code& e);

    std::string getPassword();

    std::size_t thread_pool_size_;
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    Connection_ptr new_connection_;
//    protocol_factory::AbstractRequestHandler *requestHandler;
    Factory_ptr protocolFactory;
    boost::asio::ssl::context context_;
};

} // namespace tracker
} // namespace sp2p
#endif // SP2PSERVER_H
