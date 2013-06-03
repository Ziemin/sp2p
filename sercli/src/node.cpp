#include "node.hpp"

namespace sp2p {
	namespace sercli {

		// NodeDescription
		bool NodeDescription::operator<(const NodeDescription& other) const {
			return (this->ip_address < other.ip_address);
		}

		// Node
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

		void Node::setNewDescription(NodeDescription node_desc) {
			if(this->isActive()) 
				throw NodeIsActiveException();

			this->node_desc = node_desc;
		}
	}
}
