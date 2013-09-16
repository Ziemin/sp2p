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

/**
 * @brief The Connection class - represents single connection with client. Responsible for
 * all communication.
 */
class Connection
        : public boost::enable_shared_from_this<Connection>,
        private boost::noncopyable
{
public:
    /**
     * @brief Connection - constructor of class Connectiosn
     * @param io_service - asio io_service to handle communication
     * @param context - boost ssl context
     * @param factory - class implementing AbstractFactory
     */
    explicit Connection(boost::asio::io_service& io_service, boost::asio::ssl::context &context,
                        Factory_ptr factory);

    /**
     * @brief socket - getter for socket
     * @return
     */
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type& socket();


    /**
     * @brief start - starts the first asynchronous operation for the connection.
     */
    void start();

private:

    /**
     * @brief handle_handshake - handles end of ssl handshake
     * @param e
     */
    void handle_handshake(const boost::system::error_code& e);

    /**
     * @brief handle_read - hansles completition of read operation
     * @param e
     * @param bytes_transferred
     */
    void handle_read(const boost::system::error_code& e,
                     std::size_t bytes_transferred);


    /**
    * @brief handle_write - handles completion of a write operation.
    * @param e
    */
    void handle_write(const boost::system::error_code& e);

    boost::asio::io_service::strand strand_;

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

    Factory_ptr factory;

    RequestHandler_ptr requestHandler;

    boost::array<char, 8192> buffer_;

    Request_ptr request_;

    RequestParser_ptr requestParser;

    Response_ptr reply_;

    boost::asio::streambuf outBuff;

};


typedef boost::shared_ptr<Connection> Connection_ptr;

} // namespace tracker
} // namespace sp2p
#endif // SP2PCONNECTION_H
