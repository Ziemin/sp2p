#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <memory>

#include "nodeconnection.hpp"
#include "globals.hpp"
#include "parser.hpp"
#include "sp2p_types.hpp"


namespace sp2p {
	namespace sercli {

		using namespace boost::asio::ip;
		using namespace sp2p::sercli::types;
		using namespace boost::log::trivial;

		NodeConnection::NodeConnection(ConnectionManager<NodeRequest, NodeResponse>& connection_manager) 
			: is_logged(false),
			timer(*global::io_s),
		   	strand(*global::io_s),
			socket(*global::io_s),
			connection_manager(connection_manager),
			connection(nullptr)
	   	{ }


	   bool NodeConnection::isActive() const {
		   if(connection == nullptr) return false;
		   else return connection->isActive();
	   } 

	   void NodeConnection::connect(const NodeDescription& node_desc) {
		   
		   if(!isActive()) {
			   BOOST_LOG_SEV(lg, info) << "Connecting to " << node_desc.ip_address.to_string() 
				   << " on port: " << node_desc.port;

			   boost::system::error_code ec;                                    
			   tcp::resolver resolver(*global::io_s);
			   auto endpoint_iterator = resolver.resolve({ node_desc.ip_address, node_desc.port }, ec);
			   if(ec) {
				   BOOST_LOG_SEV(lg, error) << "Could not resolve endpoint to " << node_desc.ip_address.to_string() 
					   << " on port: " << node_desc.port << " error: " << ec.message();
				   throw NodeError::NO_CONNECTION;
			   }

               boost::asio::connect(socket, endpoint_iterator, ec);
               if(ec) {
                   BOOST_LOG_SEV(lg, error) << "Could not connect to to " << node_desc.ip_address.to_string() 
                       << " on port: " << node_desc.port << " error: " << ec.message();
                   throw NodeError::NO_CONNECTION;
               }
               else {
                   BOOST_LOG_SEV(lg, info) << "Connected to " << node_desc.ip_address.to_string() 
                       << " on port: " << node_desc.port;
               }


               std::shared_ptr<Parser<NodeResponse>> parser(new NodeResponseParser);
               connection.reset
                   (
                    new Connection<NodeRequest, NodeResponse>
                    (
                     *global::io_s,
                     std::move(socket),
                     connection_manager,
                     parser,
                     global::sp2p_handler,
                     (static_cast<std::uint32_t>(TLSConType::AUTH) | static_cast<std::uint32_t>(TLSConType::CLIENT))
                    )
                   );

			   connection_manager.start(connection);

			   BOOST_LOG_SEV(lg, debug) << "End of NodeConnection::connect to " << node_desc.ip_address.to_string() 
				   << " on port: " << node_desc.port;

			   timer.expires_from_now(boost::posix_time::seconds(global::node_timeout_seconds));
			   timer.async_wait(std::bind(&NodeConnection::closeConnection, this));
		   }
	   }

	   template <typename ConnectHandler>                    
	   	   void NodeConnection::asyncConnect(const NodeDescription& node_desc, ConnectHandler con_handler) {
			   if(!isActive()) {
				   boost::asio::ip::tcp::resolver resolver(*global::io_s);
				   auto endpoint_iterator = resolver.resolve({ node_desc.ip_address, node_desc.port });

				   boost::asio::async_connect(socket, endpoint_iterator, 
						   [this] (const boost::system::error_code& ec, tcp::resolver::iterator it) 
						   {
						   		if(!ec) {
									BOOST_LOG_SEV(lg, info) << "Connected to " << node_desc.ip_address.to_string() 
									   << " on port: " << node_desc.port;

									std::shared_ptr<Parser<NodeResponse>> parser(new NodeResponseParser);
									connection.reset(
										new Connection<NodeRequest, NodeResponse>(
											*global::io_s,
											std::move(socket),
											connection_manager,
											parser,
											global::sp2p_handler,
                                            (static_cast<std::uint32_t>(TLSConType::AUTH) | static_cast<std::uint32_t>(TLSConType::CLIENT))
										));
									connection_manager.start(connection);
								} else {
									// handle connect error
								}
						   });

				   timer.expires_from_now(boost::posix_time::seconds(global::node_timeout_seconds));
				   timer.async_wait(std::bind(&NodeConnection::closeConnection, this));
			   }
		   }

	   void NodeConnection::disconnect() {
		   BOOST_LOG_SEV(lg, debug) << "Disconnecting from node: " << socket.remote_endpoint().address().to_string();

		   strand.post([this](){ connection_manager.stop(connection); });
		   connection.reset();
		   stopDeadlineTimer();
	   }


	   void NodeConnection::closeConnection() {
		   if(isActive()) {
			   BOOST_LOG_SEV(lg, debug) << "Closing connection with node";

			   connection_manager.gracefulStop(connection);
			   stopDeadlineTimer();
			   connection.reset();
		   }
	   }

	   connection_ptr<NodeRequest, NodeResponse> NodeConnection::getConnection() const {
		   return connection;
	   }

	   void NodeConnection::resetDeadlineTimer(std::uint64_t seconds) {
		   BOOST_LOG_SEV(lg, debug) << "Reseting deadline timer";
		   strand.post([this, seconds]() { 
					   timer.expires_from_now(boost::posix_time::seconds(seconds));
					   timer.async_wait(std::bind(
							   &NodeConnection::closeConnection, this));
				   });
	   }

	   void NodeConnection::stopDeadlineTimer() {
		   BOOST_LOG_SEV(lg, debug) << "Stopping deadline timer";
		   strand.post([this]() { timer.cancel(); });
	   }


		const std::string& NodeConnection::getCookie() {
            return cookie;
		}

		void NodeConnection::setCookie(std::string new_cookie) {
			cookie = std::move(new_cookie);
		}

	} /* namespace sercli */
} /* namespace sp2p */

