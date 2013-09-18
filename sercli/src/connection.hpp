#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/array.hpp>

#include <type_traits>
#include <array>
#include <memory>
#include <atomic>
#include <future>
#include <functional>

#include "connection_manager.hpp"
#include "handler.hpp"
#include "parser.hpp"
#include "message.hpp"
#include "logging.hpp"
#include "encryption.hpp"

namespace sp2p {
	namespace sercli {

        // AUTH - if authentication is required
        // CLIENT/SERVER - my role
        enum TLSConType { NO_AUTH = 1, AUTH = 2, CLIENT = 4, SERVER = 8 };

		inline void defaultHandlerMethod(boost::system::error_code /* ec */) { }

		template <typename Request, typename Response>
			class ConnectionManager;

		template <typename Request, typename Response> 
			class Handler;

		template <typename Request, typename Response>
			using handler_ptr = std::shared_ptr<Handler<Request, Response>>;


		/**
		 * Connection spawned by server
		 */
		template <typename Request, typename Response> 
			class Connection : boost::noncopyable, 
			public std::enable_shared_from_this<Connection<Request, Response>> { 

				static_assert(std::is_base_of<Message, Request>::value, "Request is not type of Message");
				static_assert(std::is_base_of<Message, Response>::value, "Response is not type of Message");

				public:
					Connection(const Connection&) = delete;
					Connection& operator=(Connection&) = delete;

					Connection(boost::asio::io_service&, 
                            boost::asio::ssl::context context,
                            boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
							ConnectionManager<Request, Response>&, 
                            parser_ptr<Response>, 
							handler_ptr<Request, Response>,
                            std::uint32_t = (TLSConType::NO_AUTH | TLSConType::CLIENT),
                            std::vector<enc::priv_st_ptr>* = nullptr,
                            std::vector<enc::cert_st_ptr>* = nullptr,
                            std::function<void()> = []()->void{});

					void setStarterFunction(std::function<void()> fun);

                    boost::system::error_code connect();

                    // handler signature: void handler(const boost::system::error_code& error)
                    template <typename ConHandler>
                        void asyncConnect(ConHandler handler);
					/**
					 * Starts connection's job
					 */
					void go();

					/**
					 * Stops connection
					 */
					void stop();

					void gracefulStop();

					std::future<std::shared_ptr<Response>> sendRequest(std::shared_ptr<Request> request);

					/*
					 * HandlerMethod has signature: handlerMethod(boost::system::error_code ec)
					 */
					template<typename SendHandler>
						void sendMessage(std::shared_ptr<Request> request, SendHandler sendHandler);

					/*
					 * HandlerMethod has signature: 
					 * handlerMethod(std::shared_ptr<Response> response,  boost::system::error_code ec);
					 */
					template <typename ReadHandler>
						void readMessage(ReadHandler readHandler);

					bool isActive();

					ConnectionManager<Request, Response>&  getConnectionManager();

				private:

					void send(std::shared_ptr<Request> request,
						   std::function<void(boost::system::error_code)> sendHandler);

					void readHandler(std::shared_ptr<Response> resp_ptr, boost::system::error_code ec,
							std::shared_ptr<std::promise<std::shared_ptr<Response>>> pr);

					void sendHandler(boost::system::error_code ec, 
							std::shared_ptr<std::promise<std::shared_ptr<Response>>> prom);

                    bool verifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx);



				private:

                    boost::asio::ssl::context context;
                    boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
                    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
					boost::asio::io_service::strand strand;
					ConnectionManager<Request, Response>& connection_manager;

					boost::array<char, 8192> recv_buf;
					boost::asio::streambuf send_buf;

					std::atomic_bool is_active;
					parser_ptr<Response> parser;
					handler_ptr<Request, Response> handler;

                    std::uint32_t con_type;
                    std::vector<enc::priv_st_ptr>* priv_keys;
                    std::vector<enc::cert_st_ptr>* certs;

					std::function<void()> starter_function;

					logging::Logger& lg = logging::sp2p_lg::get();

					std::string peer_ip;

			};

		template <typename Request, typename Response>
			using connection_ptr = std::shared_ptr<Connection<Request, Response>>;

		template <typename Request, typename Response>
			using weak_connection_ptr = std::weak_ptr<Connection<Request, Response>>;



		class SendException : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "Error occured during communication process";
				}
		};

	} /* namespace sercli */
} /* namespace sp2p */

#include "connection.cpp"

#endif /* CONNECTION_HPP */
