#ifndef MANAGER_H
#define MANAGER_H

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <botan/botan.h>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include "data.hpp"
#include "node.hpp"
#include "network.hpp"
#include "connection_manager.hpp"

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
				 * @param data_manager instance of class providing operations related to 
				 * 		  saving/serializing data
				 */
				Manager();
				Manager(DataManager& dataManager);

				void stopAll();

				/**
				 * Creates new node and adds it to base. Registers client if necessary.
				 * Synchronous version
				 */
				Node& createNode(NodeDescription node_desc);
				Node& createNode(NodeDescription node_desc, MyUser user);

				/**
				 * Returns desired nodes - self explanatory
				 */
				Node& getNode(NodeDescription node_desc) const;
				Node& getNode(std::string node_name) const;
				std::vector<node_ptr> getAllNodes() const;
				
				void removeNode(NodeDescription node_desc);
				void removeNode(std::string node_name);

				Network& createNetwork(NetworkDescription network_desc);

				/**
				 * Returns desired networks - self explanatory
				 */
				Network& getNetwork(NetworkDescription network_desc) const;
				Network& getNetwork(std::string network_name) const;
				std::vector<network_ptr> getAllNetworks() const;

				void removeNetwork(NetworkDescription node_desc);
				void removeNetwork(std::string network_name);

				/**
				 * Saves manager state 
				 */
				void saveState();

				void loadLastState();

				void setDataManager(DataManager& dataManager);


			private:
				DataManager *data_manager = nullptr;
				boost::asio::io_service &io_s;
				std::map<NodeDescription, node_ptr> nodes_map;
				std::map<NetworkDescription, network_ptr> networks_map;

				std::vector<std::shared_ptr<std::thread>> thread_pool;

				ConnectionManager<NodeRequest, NodeResponse> connection_manager;
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MANAGER_H */
