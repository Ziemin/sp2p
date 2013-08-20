#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <botan/x509_ca.h>
#include <botan/botan.h>
#include <atomic>
#include <exception>
#include <mutex>

#include "user.hpp"
#include "nodeconnection.hpp"
#include "sp2p_types.hpp"

using namespace sp2p::sercli::types;

namespace sp2p {
	namespace sercli {

		// Error from node and nodeclass

		class Node : boost::noncopyable { 
			
			typedef weak_connection_ptr<NodeRequest, NodeResponse> weak_node_con_ptr ;

			public:
				/**
				 * Constructor...
				 */
				Node(NodeDescription node_desc, ConnectionManager<NodeRequest, NodeResponse>&);

				Node(NodeDescription node_desc, ConnectionManager<NodeRequest, NodeResponse>&, 
						MyUser user);

				NodeError logIn();
				NodeError logOut();

				/**
				 * Changes password if user is registered and new username==old username
				 */
				bool setUser(MyUser user);

				NodeError registerUser();

				NodeError changePassword(const std::string& new_password);

				std::tuple<NodeError, User> getUserInfo(const NetworkDescription& network_desc, const std::string& username);

				std::tuple<NodeError, std::vector<NetworkDescription>> getNetworksList();

				std::tuple<NodeError, std::vector<NetworkDescription>> getMyNetworks();

				std::tuple<NodeError, std::vector<ServerDescription>> getServersList(const NetworkDescription& network_desc);

				/*
				 * Returns true if client is currently logged at node
				 */
				bool isActive() const;
				/** 
				 * Returns true if client is registered at this node
				 */
				bool isRegistered() const;

				std::tuple<NodeError> registerNetwork(NetworkDescription network_desc);
				std::tuple<NodeError> unregisterNetwork(NetworkDescription network_desc);

				NodeDescription getDescription();
				void setNewDescription(NodeDescription node_desc);

			private:

				NodeError beforeMessage();
			private:

				NodeDescription node_desc;
				MyUser my_user;
                NodeConnection node_connection;
				std::vector<Botan::X509_CA> ca_list;
				std::vector<Botan::X509_Certificate> user_certificates;
		};

		typedef std::shared_ptr<Node> node_ptr;

		// Error from Node class
		class NodeException : public std::exception {

			public:
				NodeException(std::string message = "Node exception happened") 
					: message(std::move(message)) { }

				virtual const char* what() const throw() {
					return message.data();
				}

			private:

				std::string message;
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_H */
