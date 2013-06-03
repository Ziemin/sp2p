#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <map>
#include <tuple>
#include "server.hpp"
#include "myserver.hpp"
#include "user.hpp"
#include "data.hpp"
#include "node.hpp"
#include "network_description.hpp"

namespace sp2p {
	namespace sercli {

		class DataManager;

		/*
		 * Class managing network existing on some nodes. It is assumed, that
		 * such network is accessible to user on all of the nodes
		 */
		class Network : boost::noncopyable {

			public:
				/**
				 * Associates node with certain network
				 * @param network_desc data identyfying network created on node
				 */
				void associateNode(const Node& node);

				/**
				 * Removes node from network
				 * @param network_id data identyfying network created on node
				 */
				void detachNode(NodeDescription node_desc);

				bool isServer() const;
				bool isClient() const;
				bool isActive() const;

				NetworkDescription getDescription() const;

                my_server_ptr becomeServer();
				std::vector<Server> getAvailableServers();

			private:

				Network(DataManager data_manager);
				
				NetworkDescription network_desc;
				std::map<NodeDescription, node_ptr> node_set;
		};

		typedef std::shared_ptr<Network> network_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NETWORK_H */
