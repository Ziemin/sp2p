#ifndef NODE_CONNECTION_HPP
#define NODE_CONNECTION_HPP

#include <string>
#include <memory>
#include <atomic>
#include <boost/noncopyable.hpp>
#include "toserverconnection.hpp"

namespace sp2p {
	namespace sercli {

		class NodeConnection : ToServerConnection, boost::noncopyable, 
		public std::enable_shared_from_this<NodeConnection> {

			public:
				NodeConnection();
 				bool isActive() const;

			private:
				std::atomic<bool> is_logged;
				std::string cookie;
		};

		typedef std::shared_ptr<NodeConnection> node_con_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_CONNECTION_HPP */
