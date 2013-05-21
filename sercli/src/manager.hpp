#ifndef MANAGER_H
#define MANAGER_H

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include "data.hpp"
#include "node.hpp"
#include "network.hpp"

namespace sp2p {
	namespace sercli {

		/**
		 * Object of Manager class is responsible for dispatching asynchronous actions
		 * related to creating networks, and assigning them with nodes
		 */
		class Manager : boost::noncopyable {

			public:
				/**
				 * Creates Manager responsible for sp2p networks control
				 *
				 * @param io_s service for asynchronous operations
				 * @param threads_num number of threads in pool
				 * @param data_manager instance of class providing operations related to 
				 * 		  saving/serializing data
				 */
				Manager(boost::asio::io_service& io_s, int threads_num, DataManager& data_manager);
				Manager(int threads_num, DataManager data_manager);

				/**
				 * Adds node to base. Registers client if necessary.
				 * Synchronous version
				 */
				void addNode(Node& node);
				/**
				 * Adds node to base. Registers client if necessary.
				 * Asynchronous version
				 */
				template<typename AddHandler>
					void addNode(Node& node, AddHandler handler);

				/**
				 * Returns desired nodes - self explanatory
				 */
				Node& getNode(NodeDescription node_desc) const;
				Node& getNode(std::string node_name) const;
				std::vector<node_ptr> getAllNodes() const;
				
				void removeNode(NodeDescription node_desc);
				void removeNode(std::string node_name);

				/**
				 * Returns desired networks - self explanatory
				 */
				Network& getNetwork(/* identyfikator - np .nazwa */) const;
				std::vector<network_ptr> getAllNetworks() const;

				void removeNetworkObject(std::string node_name);
				void removeNetworkObject(network_ptr network);

				network_ptr addNetworkObject(std::string network_name);

				/** 
				 * Registers new network on given node
				 * Synchronous
				 */
				void registerNetwork(Node& node, NetworkDescription, Network& network_object);
				/** 
				 * Registers new network on given node
				 * Asynchronous
				 */
				template <typename CreateHandler>
					void registerNetwork(Node& node, NetworkDescription, Network& network_object, CreateHandler handler);

				/** 
				 * Registers new network on given vector of nodes
				 * Synchronous
				 */
				void registerNetwork(std::vector<node_ptr>, NetworkDescription, Network& network_object);
				/** 
				 * Registers new network on given vector of nodes
				 * Asynchronous
				 */
				template <typename CreateHandler>
					void registerNetwork(std::vector<node_ptr>, NetworkDescription, Network& network_object, CreateHandler handler);

				/**
				 * Saves manager state 
				 */
				void save();

			private:
				DataManager &data_manager;
				boost::asio::io_service &io_s;

				std::map<NodeDescription, node_ptr> nodes_map;
				std::map<std::string, network_ptr> networks_map;
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MANAGER_H */
