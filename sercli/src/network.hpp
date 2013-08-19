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

				Network(NetworkDescription network_desc);
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
				const std::map<NodeDescription, node_ptr>& getAssociatedNodes() const;

                my_server_ptr becomeServer();
				std::vector<Server> getAvailableServers();

			private:

				NetworkDescription network_desc;
				std::map<NodeDescription, node_ptr> node_set;
		};

		typedef std::shared_ptr<Network> network_ptr;

		class NetworkException : public std::exception {

			public:
				NetworkException(std::string message = "Network exception happened") 
					: message(std::move(message)) { }

				virtual const char* what() const throw() {
					return message.data();
				}

			private:

				std::string message;
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NETWORK_H */
