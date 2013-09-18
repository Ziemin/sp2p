#ifndef NODE_CONNECTION_HPP
#define NODE_CONNECTION_HPP

#include <string>
#include <memory>
#include <atomic>
#include <vector>
#include <thread>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include "globals.hpp"
//#include "nodemessage.hpp"
#include "connection.hpp"
#include "sp2p_types.hpp"
#include "logging.hpp"
#include "encryption.hpp"

namespace sp2p {
    namespace sercli {

        class NodeRequest;
        class NodeResponse;

        /**
         * Wrapper over Connection<NodeRequest, NodeResponse>
         */
        class NodeConnection : boost::noncopyable, 
        public std::enable_shared_from_this<NodeConnection> {

            public:
                NodeConnection(ConnectionManager<NodeRequest, NodeResponse>& connection_manager,
                    const std::vector<enc::cert_st_ptr>& certs); 

                bool isActive() const;

                void connect(const types::NodeDescription& node_desc);

                template <typename ConnectHandler>                    
                    void asyncConnect(const types::NodeDescription& node_desc, ConnectHandler handler);

                void disconnect();

                void resetDeadlineTimer(std::uint64_t seconds = global::node_timeout_seconds);
                void stopDeadlineTimer();

                connection_ptr<NodeRequest, NodeResponse> getConnection() const;

                const std::string& getCookie();

                void setCookie(std::string new_cookie);


            private:

                void closeConnection();

            public:

                std::atomic<bool> is_logged;

            private:

                boost::asio::deadline_timer timer;
                boost::asio::io_service::strand strand;
                boost::asio::ip::tcp::socket socket;

                boost::asio::streambuf cert_buf;

                std::string cookie;

                ConnectionManager<NodeRequest, NodeResponse>& connection_manager;
                const std::vector<enc::cert_st_ptr>& certs;
                connection_ptr<NodeRequest, NodeResponse> connection;

                logging::Logger& lg = logging::sp2p_lg::get();
        };

        typedef std::shared_ptr<NodeConnection> node_con_ptr;

    } /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_CONNECTION_HPP */
