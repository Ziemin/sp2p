#include <memory>
#include <iostream>
#include <boost/asio.hpp>

#include "connection.hpp"

using namespace std::placeholders;

namespace sp2p {
	namespace sercli {

		template <typename Request, typename Response>
		   Connection<Request, Response>::Connection(
				   boost::asio::io_service& io_s,
				   boost::asio::ip::tcp::socket socket, 
				   ConnectionManager<Request, Response>& connection_manager,
				   parser_ptr<Response> parser, 
				   Handler<Request, Response>* handler,
				   std::function<void()> starter) 

		   : strand(io_s),
		   socket_(std::move(socket)),
		   connection_manager(connection_manager),
		   parser(parser),
		   handler(handler),
		   starter_function(starter)
		   {
			   is_active = true;
		   }

		template <typename Request, typename Response>
			void Connection<Request, Response>::go() {

				starter_function();
			}


		template <typename Request, typename Response>
			void Connection<Request, Response>::stop() {

				strand.post([&]()
				{ 
					if(!socket_.is_open()) {
						socket_.close();
						is_active = false;
					}
				});
			}

		template <typename Request, typename Response>
			void Connection<Request, Response>::gracefulStop() {

				strand.post([&]()
				{ 
					if(!socket_.is_open()) {
						boost::system::error_code ign_error;
						socket_.shutdown(socket_.shutdown_both, ign_error);
						is_active = false;
					}
				});
			}


		template <typename Request, typename Response>
			std::future<Response> Connection<Request, Response>::sendRequest(std::shared_ptr<Request> request) {

				std::promise<Response> pr;
				std::future<Response> response = pr.get_future();

				sendMessage(std::forward<Request>(request),  // sendMessage handler
						std::bind(
							[this] (std::promise<Response> pr) 
							{
                            	readMessage(
									std::bind( // readMessage handler
                                          	[](Response response, std::promise<Response> pr) 
											{
												pr.set_exception(SendException());
												pr.set_value(response);
											},
											_1, std::move(pr)
										));
							},
							std::move(pr))
						);

				return response;
			}


		template <typename Request, typename Response>
			template <typename HandlerMethod>
			void Connection<Request, Response>::sendMessage(std::shared_ptr<Request> request, 
					HandlerMethod handlerMethod) 
			{
				strand.post(
						std::bind(&Connection::send, 
							this->shared_from_this(), 
							std::forward<Request>(request), 
							handlerMethod));
			}

		template <typename Request, typename Response>
			template <typename HandlerMethod>
			void Connection<Request, Response>::send(std::shared_ptr<Request> request, HandlerMethod handlerMethod) {

				std::ostream os(&send_buf);
				os << (std::uint32_t) request->ByteSize();
				request->SerializeToOstream(&os);

				auto self(this->shared_from_this());
				boost::asio::async_write(socket_, send_buf, strand.wrap(
						[this, self] (boost::system::error_code ec, std::size_t length)
						{
							//send_buf.consume(length);

							if(!ec) handlerMethod();
							else handler->handleSendError(*this, request);

						}));
			}

		template <typename Request, typename Response> 
			template <typename HandlerMethod>
			void Connection<Request, Response>::readMessage(HandlerMethod handlerMethod) {
				
				auto self(this->shared_from_this());
				socket_.async_read_some(
						boost::asio::buffer(recv_buf), strand.wrap(
						[this, self] (boost::system::error_code ec, std::size_t length) 
						{
							if(!ec) {
								Response response;
                               	parse_result result = parser->parse(response, recv_buf.data(), length);
								if(result == parse_result::GOOD) {
								   	handler->handleResponse(*this, response); 
								}
								else if(result == parse_result::BAD) {
									handler->handleParseError(*this);
								}
								else 
									readMessage(handlerMethod);
							}
							else {
								handler->handleReadError(*this);
							}
						}));
			}

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
