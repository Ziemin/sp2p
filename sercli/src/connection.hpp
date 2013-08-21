#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

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

namespace sp2p {
	namespace sercli {

		void defaultHandlerMethod() { }

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

					Connection(boost::asio::io_service&, boost::asio::ip::tcp::socket, 
							ConnectionManager<Request, Response>&, parser_ptr<Response>, 
							Handler<Request, Response>*, std::function<void()>);

					/**
					 * Starts connection's job
					 */
					void go();

					/**
					 * Stops connection
					 */
					void stop();

					void gracefulStop();

					std::future<Response> sendRequest(std::shared_ptr<Request> request);

					/*
					 * HandlerMethod has signature: handlerMethod()
					 */
					template <typename HandlerMethod>
						void sendMessage(std::shared_ptr<Request> request, HandlerMethod handlerMethod=defaultHandlerMethod);

					/*
					 * HandlerMethod has signature: handlerMethod(Response response);
					 */
					template <typename HandlerMethod>
						void readMessage(HandlerMethod handlerMethod);

					bool isActive();

					ConnectionManager<Request, Response>&  getConnectionManager();

				private:

					template <typename HandlerMethod>
						void send(std::shared_ptr<Request> request, HandlerMethod handlerMethod);

				private:

					ConnectionManager<Request, Response>& connection_manager;
					boost::asio::ip::tcp::socket socket_;
					boost::asio::io_service::strand strand;

					std::array<char, 8192> recv_buf;
					boost::asio::streambuf send_buf;

					std::atomic_bool is_active;
					parser_ptr<Response> parser;
					Handler<Request, Response>* handler;

					std::function<void()> starter_function;

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

#endif /* CONNECTION_HPP */
