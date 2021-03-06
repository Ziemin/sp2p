#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <exception>
#include <functional>

#include <boost/log/core.hpp>

#include "connection.hpp"

namespace pl = std::placeholders;

namespace sp2p {
    namespace sercli {

    using namespace boost::log::trivial;

        template <typename Request, typename Response>
           Connection<Request, Response>::Connection(
                   boost::asio::io_service& io_s,
                   boost::asio::ssl::context context,
                   boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
                   ConnectionManager<Request, Response>& connection_manager,
                   parser_ptr<Response> parser, 
                   handler_ptr<Request, Response> handler,
                   std::uint32_t con_type,
                   const std::vector<enc::priv_st_ptr>* priv_keys,
                   const std::vector<enc::cert_st_ptr>* certs,
                   std::function<void()> starter) 

           : context(std::move(context)),
           endpoint_iterator(endpoint_iterator),
           socket(io_s, this->context),
           strand(io_s),
           connection_manager(connection_manager),
           parser(parser),
           handler(handler),
           con_type(con_type),
           priv_keys(priv_keys),
           certs(certs),
           starter_function(starter)
           {
               is_active = true;

               peer_ip = endpoint_iterator->endpoint().address().to_string();
               BOOST_LOG_SEV(lg, debug) << "Created Connection object with: " << peer_ip;
               if(con_type & TLSConType::AUTH) {
                   if(certs != nullptr && certs->size() > 0) {
                       BOOST_LOG_SEV(lg, debug) << "Loading verify files" << peer_ip;
                       enc::cert_st_ptr cs = (*certs)[0];

                       if(cs->getPath().size() > 0 && cs->inFile(cs->getPath()))
                           this->context.load_verify_file(cs->getPath()+"/"+cs->getFilename());
                   }
                   socket.set_verify_callback(std::bind(
                               &Connection<Request,Response>::verifyCertificate,
                               this, pl::_1, pl::_2));
               }
           }


        template <typename Request, typename Response>
            boost::system::error_code Connection<Request, Response>::connect() {
               BOOST_LOG_SEV(lg, debug) << "Sync connecting with: " << peer_ip;

               boost::system::error_code ec;
               boost::asio::connect(socket.lowest_layer(), endpoint_iterator, ec);
               if(ec) {
                   BOOST_LOG_SEV(lg, trace) << "Could not connect to: " << peer_ip;
                   return ec;
               } else {
                   BOOST_LOG_SEV(lg, trace) << "Connected to: " << peer_ip;
               }

               if(con_type & TLSConType::CLIENT) {
                   socket.handshake(boost::asio::ssl::stream_base::client, ec);
                   if(ec) BOOST_LOG_SEV(lg, trace) << "Handshake failed with: " << peer_ip;
               }
               else if(con_type & TLSConType::SERVER) {
                   socket.handshake(boost::asio::ssl::stream_base::server, ec);
                   if(ec) BOOST_LOG_SEV(lg, trace) << "Handshake failed with: " << peer_ip;
               } 
               else {
                   BOOST_LOG_SEV(lg, error) << "No role for connection specified" << std::endl;
                   ec.assign(boost::system::errc::invalid_argument, ec.category());
               }
               return ec;
            }

        template <typename Request, typename Response>
            template <typename ConHandler>
            void Connection<Request, Response>::asyncConnect(ConHandler handler) {
               BOOST_LOG_SEV(lg, debug) << "Async connecting with: " << peer_ip;

               auto self(this->shared_from_this());
               boost::asio::async_connect(socket.lowest_layer(), endpoint_iterator, 
                       [this, self, handler](const boost::system::error_code& ec) {
                            if(!ec) {
                                BOOST_LOG_SEV(lg, info) << "Connected to: " << peer_ip;

                                if(con_type & TLSConType::CLIENT) 
                                    socket.async_handshake(boost::asio::ssl::stream_base::client, handler);
                                else if(con_type & TLSConType::SERVER) 
                                    socket.async_handshake(boost::asio::ssl::stream_base::server, handler);
                                else {
                                    boost::system::error_code error;
                                    BOOST_LOG_SEV(lg, error) << "No role for connection specified" << std::endl;
                                    error.assign(boost::system::errc::invalid_argument, ec.category());
                                }
                            } else {
                                BOOST_LOG_SEV(lg, info) << "Could not connect to: " << peer_ip;
                            }
                       });
            }

        template <typename Request, typename Response>
            bool Connection<Request, Response>::verifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx) {
                BOOST_LOG_SEV(lg, trace) << "Veryfing certificate of: " << peer_ip;
                return preverified;
            }

        template <typename Request, typename Response>
            void Connection<Request, Response>::go() {
                BOOST_LOG_SEV(lg, trace) << "Started connection with: " << peer_ip;
                starter_function();
            }

        template <typename Request, typename Response>
            void Connection<Request, Response>::setStarterFunction(std::function<void()> fun) {
                starter_function = fun;
            }


        template <typename Request, typename Response>
            void Connection<Request, Response>::stop() {

                BOOST_LOG_SEV(lg, trace) << "Stopping connection with: " << peer_ip;
                strand.post([this]()
                { 
                    if(this->socket.lowest_layer().is_open()) {
                        this->socket.lowest_layer().close();
                        this->is_active = false;
                    }
                });
            }

        template <typename Request, typename Response>
            void Connection<Request, Response>::gracefulStop() {

                BOOST_LOG_SEV(lg, trace) << "Gracefully stopping connection with: " << peer_ip;
                strand.post([this]()
                { 
                    if(this->socket.lowest_layer().is_open()) {
                        boost::system::error_code ign_error;
                        this->socket.lowest_layer().shutdown(socket.lowest_layer().shutdown_both, ign_error);
                        this->is_active = false;
                    }
                });
            }


        template <typename Request, typename Response>
            std::future<std::shared_ptr<Response>> Connection<Request, Response>::sendRequest(std::shared_ptr<Request> request) {

                BOOST_LOG_SEV(lg, trace) << "Sync-Sending request to: " << peer_ip;

                std::shared_ptr<std::promise<std::shared_ptr<Response>>> pr = 
                    std::make_shared<std::promise<std::shared_ptr<Response>>>();

                std::future<std::shared_ptr<Response>> response = pr->get_future();

                sendMessage(request, 
                        std::bind(&Connection<Request, Response>::sendHandler, this->shared_from_this(), 
                            pl::_1,
                            pr));

                return response;
            }


        template <typename Request, typename Response>
            template <typename SendHandler>
            void Connection<Request, Response>::sendMessage(std::shared_ptr<Request> request, SendHandler sendHandler) {
                BOOST_LOG_SEV(lg, trace) << "Send message to: " << peer_ip;

                strand.post([this, request, sendHandler]() -> void {
                           this->send(request, sendHandler);
                        });
            }

        template <typename Request, typename Response>
            void Connection<Request, Response>::send(std::shared_ptr<Request> request,
                   std::function<void(boost::system::error_code)> newSendHandler) {

                BOOST_LOG_SEV(lg, trace) << "Send function - building message for: " << peer_ip;
                std::ostream os(&send_buf);
                std::uint32_t message_size = request->ByteSize();
                os.write(reinterpret_cast<const char*>(&message_size), sizeof(std::uint32_t));
                request->SerializeToOstream(&os);

                auto self(this->shared_from_this());
                boost::asio::async_write(socket, send_buf, strand.wrap(
                        [this, self, newSendHandler, request] (boost::system::error_code ec, std::size_t length)
                        {
                            BOOST_LOG_SEV(lg, trace) << "Async write handler";
                            newSendHandler(ec);
                            if(ec) handler->handleSendError(*this, *request);

                        }));
            }

        template <typename Request, typename Response> 
            template <typename ReadHandler>
            void Connection<Request, Response>::readMessage(ReadHandler readHandler) {
                
                BOOST_LOG_SEV(lg, trace) << "Reading message from: " << peer_ip;
                auto self(this->shared_from_this());
                socket.async_read_some(
                        boost::asio::buffer(recv_buf), strand.wrap(
                        [this, self, readHandler] (boost::system::error_code ec, std::size_t length) 
                        {
                            std::shared_ptr<Response> response_ptr;
                            if(!ec) {
                                BOOST_LOG_SEV(lg, debug) << "Read " << length << 
                                    " bytes from: " << peer_ip;

                                response_ptr.reset(new Response());
                                parse_result result = parser->parse(*response_ptr, recv_buf.data(), length);
                                if(result == parse_result::GOOD) {
                                    handler->handleResponse(*this, *response_ptr); 
                                    readHandler(response_ptr, ec);
                                }
                                else if(result == parse_result::BAD) {
                                    BOOST_LOG_SEV(lg, trace) << "Could not parse message from: " << peer_ip;

                                    handler->handleParseError(*this);
                                    boost::system::error_code error;
                                    error.assign(boost::asio::error::fault, ec.category());
                                    readHandler(response_ptr, error);
                                }
                                else 
                                    readMessage(readHandler);
                            }
                            else {
                                handler->handleReadError(*this);
                                readHandler(response_ptr, ec);
                            }
                        }));
            }

        template <typename Request, typename Response> 
                void Connection<Request, Response>::sendHandler(boost::system::error_code ec, 
                        std::shared_ptr<std::promise<std::shared_ptr<Response>>> prom) {
                    BOOST_LOG_SEV(lg, debug) << "Send handler";
                    if(ec) {
                        prom->set_exception(std::make_exception_ptr(SendException()));
                    }
                    else {
                        readMessage(
                                std::bind(&Connection<Request, Response>::readHandler, this->shared_from_this(),
                                    pl::_1, 
                                    pl::_2,
                                    prom));
                    }
                }

        template <typename Request, typename Response> 
            void Connection<Request, Response>::readHandler(std::shared_ptr<Response> resp_ptr, boost::system::error_code ec, 
                    std::shared_ptr<std::promise<std::shared_ptr<Response>>> pr) {

                BOOST_LOG_SEV(lg, debug) << "Read handler";
                if(ec) pr->set_exception(std::make_exception_ptr(SendException()));
                else pr->set_value(resp_ptr);
            }

        // implementations
        //template class Connection<NodeRequest, NodeResponse>;

        template <typename Request, typename Response> 
            bool Connection<Request, Response>::isActive() {
                return is_active;
            }

        template <typename Request, typename Response> 
            ConnectionManager<Request, Response>& Connection<Request, Response>::getConnectionManager() {
                return connection_manager;
            }


    } /* namespace sercli */
} /* namespace sp2p */
