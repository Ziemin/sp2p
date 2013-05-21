#ifndef NODE_CONNECTION_HPP
#define NODE_CONNECTION_HPP

#include <memory>
#include <boost/noncopyable.hpp>

namespace sp2p {
	namespace sercli {


		class NodeConnection : boost::noncopyable, public std::enable_shared_from_this<NodeConnection {
			
			public:

		};

		typedef std::shared_ptr<NodeConnection> node_con_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_CONNECTION_HPP */
