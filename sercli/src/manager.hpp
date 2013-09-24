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
#include "connection_manager.hpp"

#ifdef LOGGING_ENABLED
#include "logging.hpp"
#endif

namespace sp2p {
	namespace sercli {

		/**
		 * Object of Manager class is responsible for managing whole sp2p related data
		 * including creating networks, and assigning nodes to them
		 */
		class Manager : boost::noncopyable {

			public:
				/**
				 * Creates Manager responsible for sp2p networks control
				 *
				 * @param data_manager instance of class providing operations related to 
				 *		  saving/serializing data
				 */
				Manager(DataManager& dataManager);

				//!< Stops all connections 
				void stopAll();

				node_ptr createNode(const types::NodeDescription& node_desc);
				node_ptr createNode(const types::NodeDescription& node_desc, MyUser user);

				node_ptr getNode(const types::NodeDescription& node_desc) const;
				node_ptr getNode(const std::string& node_name) const;
				std::vector<node_ptr> getAllNodes() const;
				
				void removeNode(const types::NodeDescription& node_desc);
				void removeNode(const std::string& node_name);

				network_ptr createNetwork(const types::NetworkDescription& network_desc);

				network_ptr getNetwork(const types::NetworkDescription& network_desc) const;
				network_ptr getNetwork(const std::string& network_name) const;
				std::vector<network_ptr> getAllNetworks() const;

				void removeNetwork(const types::NetworkDescription& node_desc);
				void removeNetwork(const std::string& network_name);

				/**
				 * Saves manager state
				 * both information about nodes/networks and private keys/certificates
				 */
				void saveState();
				/**
				 * Saves manager state
				 * both information about nodes/networks and private keys/certificates
				 */
				void saveState(const std::string& dest_file);

				/**
				 * Loads manager state from disk
				 */
				void loadState();
				/**
				 * Loads manager state from disk
				 */
				void loadState(const std::string& source_file);

				/**
				 * Clears manager's data
				 */
				void clear();

				void setDataManager(DataManager& dataManager);



			private:
				DataManager& data_manager;
				std::map<types::NodeDescription, node_ptr> nodes_map;
				std::map<types::NetworkDescription, network_ptr> networks_map;

				std::vector<std::shared_ptr<std::thread>> thread_pool;

				ConnectionManager<NodeRequest, NodeResponse> connection_manager;

				logging::Logger& lg = logging::sp2p_lg::get();
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MANAGER_H */
