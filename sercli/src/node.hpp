#ifndef NODE_H
#define NODE_H

#include <memory>
#include <boost/noncopyable.hpp>
#include <atomic>
#include "user.hpp"
#include "network.hpp"
#include "nodeconnection.hpp"

namespace sp2p {
	namespace sercli {

		struct NodeDescription {
	
			bool operator<(const NodeDescription& other) const;
		};

		class Node : boost::noncopyable { 
			
			public:
				/**
				 * Constructor...
				 */
				Node(NodeDescription node_desc, MyUser user, bool registered);

				void logIn();
				void logOut();

				/**
				 * Changes password if user is registered and new username==old username
				 */
				void setUser(MyUser user);
				/*
				 * Returns true if client is currently logged at node
				 */
				bool isActive() const;
				/** 
				 * Returns true if client is registered at this node
				 */
				bool isRegistered() const;

				void registerNetwork(NetworkDescription network_desc);
				void unregisterNetwork(NetworkDescription network_desc);

				NodeDescription getDescription();

			private:
				NodeDescription node_desc;
				MyUser my_user;
				bool is_registered;
                node_con_ptr node_connection;

		};

		typedef std::shared_ptr<Node> node_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_H */
