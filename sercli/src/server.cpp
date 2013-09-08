#include "server.hpp"

#include <signal.h>
#include <ctime>
#include <climits>
#include <functional>

using namespace std::placeholders;

namespace sp2p {
	namespace sercli {

        template<typename Request, typename Response> 
            Server<Request, Response>::Server(boost::asio::io_service& io_s,
                    const tcp::endpoint& endpoint,
                    const ParserFactory<Response>& parser_factory,
                    handler_ptr<Request, Response> handler) : 
                acceptor_(io_s),
                socket_(io_s),
                strand_(io_s),
                endpoint_(endpoint),
                timer(io_s),
                signals(io_s),
                parser_factory(parser_factory),
                handler(handler)
        {
            signals.add(SIGINT); 
            signals.add(SIGTERM);
            signals.add(SIGQUIT);
        }

        template<typename Request, typename Response>
            void Server<Request, Response>::start() {

                await_stop();
                start_acceptor();
                accept();
                updateState();
            }

        template<typename Request, typename Response> 
            void Server<Request, Response>::start_acceptor() {

                acceptor_.open(endpoint_.protocol());
                acceptor_.set_option(tcp::acceptor::reuse_address(true));
                acceptor_.bind(endpoint_);
                acceptor_.listen();
            }

        template<typename Request, typename Response>
            void Server<Request, Response>::accept() {

                auto self(this->shared_from_this());
                acceptor_.async_accept(socket_, strand_.wrap(
                        [this, self](boost::system::error_code ec) 
                        {
                            if(!acceptor_.is_open()) {
                                return;
                            }

                            if(!ec) {
                                connection_ptr<Request, Response> con(new Connection<Request, Response>(
                                        std::move(socket_), 
                                        connection_manager,
                                        parser_factory.getParser(),
                                        handler));

                                con->setStarterFunction(std::bind(
                                    Connection<Request, Response>::readMessage, con, [](){}));

                                connection_manager.start(con);
                            }
                            accept();
                        }));
            }

        template<typename Request, typename Response>
            void Server<Request, Response>::stop() {
                auto self(this->shared_from_this());
                strand_.dispatch([this, self]() 
                        {
                            acceptor_.close();
                            connection_manager.stopAll();
                        });
            }

        template<typename Request, typename Response> 
            void Server<Request, Response>::changeIP(const tcp::endpoint& endpoint) {
                endpoint_ = endpoint;

                auto self(this->shared_from_this);
                strand_.dispatch([this, self]() 
                        {
                            acceptor_.close();
                            start_acceptor();
                            accept();
                            updateState();
                        });
            }

        template<typename Request, typename Response>
            void Server<Request, Response>::await_stop() {
                auto self(this->shared_from_this);
                signals.async_wait([this, self](boost::system::error_code /* ec */, int /* sig_number */) 
                        {
                            acceptor_.close();
                            connection_manager.stopAll();
                        });
            }

        template<typename Request, typename Response>
            network_ptr Server<Request, Response>::getRelatedNetwork() const {
                return network;
            }

        template<typename Request, typename Response>
            int Server<Request, Response>::allNodes() const {
                return network->getAssociatedNodes().size();
            }

        template<typename Request, typename Response>
            int Server<Request, Response>::connectedNodes() const {
                return activeNodes;
            }

        template<typename Request, typename Response>
            void Server<Request, Response>::updateState() {

                auto fun = std::bind(Server<Request, Response>::updateState(), 
                        this->shared_from_this(), _1, _2, _3);
                for(auto node_it: network->getAssociatedNodes()) {
                    node_ptr node = node_it.second.lock();
                    node->asyncUpdateServer(network->getDescription(), endpoint_.port(),
                            strand_.wrap(
                                std::bind(fun, _1, _2, node)));
                }
            }

        template<typename Request, typename Response>
            void Server<Request, Response>::updateStateHandler(NodeError error, std::int32_t time_left, node_ptr node) {
                if(any(error)) {
                    return;
                } else {
                    // searching for given noded in list
                    auto it = nodes_to_update.begin();
                    while(it != nodes_to_update.end()) {
                        if(it->second.get() == node) {
                            nodes_to_update.erase(it);
                            break;
                        }
                        ++it;
                    }
                    std::uint64_t earliest_update = std::numeric_limits<std::uint64_t>::max();

                    if(!nodes_to_update.empty()) 
                        earliest_update = nodes_to_update.begin()->first;

                    std::uint64_t this_update = std::time(0) + time_left;
                    nodes_to_update.insert(std::make_pair(this_update, node));

                    if(earliest_update > this_update) {
                        // setup timer
                        timer.expires_at(timer.expires_at() + boost::posix_time::seconds(time_left>>1));
                        timer.async_wait(std::bind(Server<Request, Response>::timerUpdate(), this->shared_from_this()));
                    }
                }

            }

        template<typename Request, typename Response>
            void Server<Request, Response>::timerUpdate() {

                auto fun = std::bind(Server<Request, Response>::updateState(), 
                        this->shared_from_this(), _1, _2, _3);
                for(auto node_it: nodes_to_update) {
                     // update all TODO update according to time left
                     node_it.second->asyncUpdateServer(network->getDescription(), endpoint_.port(),
                            strand_.wrap(
                                std::bind(fun, _1, _2, node_it.second)));
                     
                }
            }

    } /* namespace sercli */
} /* namespace sp2p */
