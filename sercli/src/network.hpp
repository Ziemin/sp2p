#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <map>
#include <tuple>
#include "node.hpp"
#include "server.hpp"
#include "client.hpp"
#include "user.hpp"
#include "data.hpp"

namespace sp2p {
	namespace sercli {

		namespace network {

			enum class AccessRights { PUBLIC, PRIVATE };
			enum class Visibility { LISTED, UNLISTED };
			enum class ParticipationRights { CLIENT_ONLY, CLIENT_SERVER };

		} /* namespace network_utilities */

		struct NetworkDescription {
			network::AccessRights access_rights;
			network::Visibility  visability;
			network::ParticipationRights participation_rights;

			User creator;
		};

		class Network : boost::noncopyable {

			friend class Client;
			friend class Server;
			friend class Manager;

			public:
				/**
				 * Associates node with certain network
				 * @param network_desc data identyfying network created on node
				 */
				void associateNode(const Node& node, NetworkDescription network_desc);

				/**
				 * Removes node from network
				 * @param network_id data identyfying network created on node
				 */
				void detachNode(NodeDescription node_desc);
				void detachNode(std::string node_name);

				bool isServer() const;
				bool isClient() const;
				bool isActive() const;

				std::string getName() const;

				Client getClient(); 
                Server getServer();

			private:

				Network(std::string name, DataManager data_manager);

				std::string network_name;
				std::map<NetworkDescription, node_ptr> nodes_map;

		};

		typedef std::shared_ptr<Network> network_ptr;

		//class ServerNetwork : public Network

	} /* namespace sercli */

} /* namespace sp2p */

#endif /* NETWORK_H */
