#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include "node.hpp"

namespace sp2p {
	namespace sercli {

		namespace network_utilities {
			enum class AccessRights { };
			enum class Visibility { };
			enum class ParticipationRights { };
		} /* namespace network_utilities */

		class Network {

			public:
				template <typename AssociateHandler>
					void associateNode(Node& node, std::string network_id, AssociateHandler handler);

				template <typename DetachHandler>
					void detachNode(std::string node_name, DetachHandler handler);

				bool isServer();

				void save();

			private:
		};

		//class ServerNetwork : public Network

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NETWORK_H */
