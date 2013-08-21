#include <boost/date_time/posix_time/posix_time.hpp>
#include <memory>

#include "nodeconnection.hpp"
#include "globals.hpp"
#include "parser.hpp"
#include "sp2p_types.hpp"

using namespace boost::asio::ip;
using namespace sp2p::sercli::types;

namespace sp2p {
	namespace sercli {

		NodeConnection::NodeConnection(ConnectionManager<NodeRequest, NodeResponse>& connection_manager) 
			: timer(*global::io_s),
		   	strand(*global::io_s),
			socket(*global::io_s),
			connection_manager(connection_manager),
			connection(nullptr)
	   	{ }


	   bool NodeConnection::isActive() const {
		   return (connection != nullptr || connection->isActive());
	   } 

	   void NodeConnection::connect(const NodeDescription& node_desc) {
		   
		   if(!isActive()) {
			   tcp::resolver resolver(*global::io_s);
			   auto endpoint_iterator = resolver.resolve({ node_desc.ip_address, node_desc.port });

			   boost::system::error_code ec;
			   boost::asio::connect(socket, endpoint_iterator, ec);

			   if(ec) throw NodeError::NO_CONNECTION;

			   std::shared_ptr<NodeResponseParser> parser(new NodeResponseParser);
			   connection.reset(
					   new Connection<NodeRequest, NodeResponse>(
						   *global::io_s,
						   std::move(socket),
						   connection_manager,
						   std::dynamic_pointer_cast<Parser<NodeResponse>>(parser),
						   global::sp2p_handler,
						   [](){} // do nothing
						   ));

			   connection_manager.start(connection);

			   timer.async_wait(std::bind(&NodeConnection::closeConnection, shared_from_this()));
		   }
	   }

	   template <typename ConnectHandler>                    
	   	   void NodeConnection::asyncConnect(const NodeDescription& node_desc, ConnectHandler handler) {
			   if(!isActive()) {
				   boost::asio::ip::tcp::resolver resolver(*global::io_s);
				   auto endpoint_iterator = resolver.resolve({ node_desc.ip_address, node_desc.port });

				   auto self(shared_from_this());
				   boost::asio::async_connect(socket, endpoint_iterator, 
						   [&, self] (const boost::system::error_code& ec, tcp::resolver::iterator it) 
						   {
						   		if(!ec) {
									std::shared_ptr<NodeResponseParser> parser(new NodeResponseParser);

									connection.reset(
										new Connection<NodeRequest, NodeResponse>(
											*global::io_s,
											std::move(socket),
											connection_manager,
											std::dynamic_pointer_cast<Parser<NodeResponse>>(parser),
											global::sp2p_handler,
											handler
											));

									connection_manager.start(connection);
								} else {
									// handle connect error
								}
						   });

				   timer.async_wait(std::bind(&NodeConnection::closeConnection, shared_from_this()));
			   }
		   }

	   void NodeConnection::disconnect() {
		   auto self(shared_from_this());
		   strand.post([&, self]() { connection_manager.stop(connection); });
	   }


	   void NodeConnection::closeConnection() {
		   if(isActive()) {
			   connection_manager.gracefulStop(connection);
			   is_logged = false;
		   }
	   }

	   connection_ptr<NodeRequest, NodeResponse> NodeConnection::getConnection() const {
		   return connection;
	   }

	   void NodeConnection::resetDeadlineTimer(std::uint64_t seconds) {
		   auto self(shared_from_this());
		   strand.post([&, self]() { 
					   timer.expires_at(timer.expires_at() + boost::posix_time::seconds(seconds)); 
					   timer.async_wait(std::bind(
							   &NodeConnection::closeConnection, shared_from_this()));
				   });
	   }

	   void NodeConnection::stopDeadlineTimer() {
		   auto self(shared_from_this());
		   strand.post([&, self]() { timer.cancel(); });
	   }


		const std::string& NodeConnection::getCookie() {
            return cookie;
		}

		void NodeConnection::setCookie(std::string new_cookie) {
			cookie = std::move(new_cookie);
		}

	} /* namespace sercli */
} /* namespace sp2p */

