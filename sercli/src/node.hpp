#ifndef NODE_H
#define NODE_H

#include <memory>
#include <boost/noncopyable.hpp>
#include "user.hpp"

namespace sp2p {
	namespace sercli {

		struct NodeDescription {
	
		};

		class Node : boost::noncopyable {
			
			public:
				/**
				 * Constructor...
				 */
				Node(NodeDescription node_desc, MyUser user, bool registered);

				void logIn();
				void logOut();

				void setUser(MyUser user);
				/*
				 * Returns true if client is currently logged at node
				 */
				bool isActive() const;
				/** 
				 * Returns true if client is registered at this node
				 */
				bool isRegisterd() const;

				NodeDescription getDescription();

			private:
				NodeDescription node_desc;
                bool isRegistered;

		};

		typedef std::shared_ptr<Node> node_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_H */
