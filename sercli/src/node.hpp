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
#include "node_description.hpp"
#include "network_description.hpp"

namespace sp2p {
	namespace sercli {

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
				void setNewDescription(NodeDescription node_desc);

			private:

				NodeDescription node_desc;
				MyUser my_user;
				bool is_registered;
                node_con_ptr node_connection;
				std::vector<Botan::X509_CA> ca_list;
				std::vector<Botan::X509_Certificate> user_certificates;

				std::mutex node_mutex;
		};

		typedef std::shared_ptr<Node> node_ptr;


		class NodeException : public std::exception {
			virtual const char* what() const throw() {
				return "NodeException happend";
			}
		};

        class NodeIsActiveException : public NodeException {
			virtual const char* what() const throw() {
				return "Node is active at the moment";
			}
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* NODE_H */
