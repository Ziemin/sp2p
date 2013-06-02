#include "node.hpp"

namespace sp2p {
	namespace sercli {

		Node::Node(NodeDescription node_desc, MyUser user, bool registered) 
			: node_desc(node_desc), 
			my_user(user), 
			is_registered(registered)
		{ 
		}

		bool Node::isActive() const {
			return node_connection->isActive();
		}

		bool Node::isRegistered() const {
			return this->is_registered; 
		}
	}
}
