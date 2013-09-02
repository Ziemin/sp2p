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
				 *		  saving/serializing data
				 */
				Manager();
				Manager(DataManager& dataManager);

				void stopAll();

				/**
				 * Creates new node and adds it to base. Registers client if necessary.
				 * Synchronous version
				 */
				node_ptr createNode(const types::NodeDescription& node_desc);
				node_ptr createNode(const types::NodeDescription& node_desc, MyUser user);

				/**
				 * Returns desired nodes - self explanatory
				 */
				node_ptr getNode(const types::NodeDescription& node_desc) const;
				node_ptr getNode(const std::string& node_name) const;
				std::vector<node_ptr> getAllNodes() const;
				
				void removeNode(const types::NodeDescription& node_desc);
				void removeNode(const std::string& node_name);

				network_ptr createNetwork(const types::NetworkDescription& network_desc);

				/**
				 * Returns desired networks - self explanatory
				 */
				network_ptr getNetwork(const types::NetworkDescription& network_desc) const;
				network_ptr getNetwork(const std::string& network_name) const;
				std::vector<network_ptr> getAllNetworks() const;

				void removeNetwork(const types::NetworkDescription& node_desc);
				void removeNetwork(const std::string& network_name);

				/**
				 * Saves manager state 
				 */
				void saveState();

				void loadLastState();

				void setDataManager(DataManager& dataManager);


			private:
				DataManager *data_manager = nullptr;
				boost::asio::io_service &io_s;
				std::map<types::NodeDescription, node_ptr> nodes_map;
				std::map<types::NetworkDescription, network_ptr> networks_map;

				std::vector<std::shared_ptr<std::thread>> thread_pool;

				ConnectionManager<NodeRequest, NodeResponse> connection_manager;
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MANAGER_H */
