#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <atomic>

#include "network.hpp"
#include "connection_manager.hpp"
#include "parser.hpp"
#include "handler.hpp"
#include "parser_factory.hpp"

using boost::asio::ip::tcp;

namespace sp2p {
    namespace sercli {

        /**
         * Class representing basic server
         */
        template<typename Request, typename Response>
            class Server : std::enable_shared_from_this<Server<Request, Response>> {

                public:

                    Server(boost::asio::io_service& io, 
                           const tcp::endpoint& endpoint,
                           const ParserFactory<Response>& parser_factory,
                           handler_ptr<Request, Response> handler);

                    void start();
                    void changeIP(const tcp::endpoint& endpoint);
                    void stop();

                    int connectedNodes() const;
                    int allNodes() const;

                    network_ptr getRelatedNetwork() const;

                protected:

                    virtual void accept();

                    void updateState();
                    void timerUpdate();

                    virtual void updateStateHandler(Node* node, types::NodeError error, std::int32_t time_left);
                    virtual void connectionErrorHandler();

                private:

                    void await_stop();
                    void start_acceptor();

                protected:

                    tcp::acceptor acceptor_;
                    tcp::socket socket_;
                    boost::asio::io_service::strand strand_;
                    tcp::endpoint endpoint_;
                    boost::asio::deadline_timer timer;

                    network_ptr network;
                    ConnectionManager<Request, Response> connection_manager;
                    const ParserFactory<Response>& parser_factory;
                    handler_ptr<Request, Response> handler;

                    std::atomic_int activeNodes;

                private:

                    std::map<std::uint64_t, Node*> nodes_to_update;
                    boost::asio::signal_set signals;
            };

    } /* namespace sercli */
} /* namespace sp2p */

#include "server.cpp"

#endif /* SERVER_H */
