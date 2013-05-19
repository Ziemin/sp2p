#ifndef NODE_H
#define NODE_H

#include <memory>

namespace sp2p {
	namespace sercli {

		class Node {
			
			public:
				void login();
				void logout();
				bool isActive() const;
				void save();
		};

		typedef std::shared_ptr<Node> node_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_H */
