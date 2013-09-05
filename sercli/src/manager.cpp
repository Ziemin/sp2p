#include <thread>
#include <boost/asio.hpp>
#include <vector>

#include "manager.hpp"
#include "globals.hpp"


namespace sp2p {
	namespace sercli {

		Manager::Manager(DataManager& dataManager) : data_manager(dataManager) { }

		void Manager::stopAll() {
			for(auto node_pair: nodes_map) node_pair.second->stopConnections();
		}

		node_ptr Manager::createNode(const NodeDescription& node_desc) {
			if(nodes_map.find(node_desc) != nodes_map.end())
				throw NodeException("Node with such description already exists");

			node_ptr node(new Node(node_desc, connection_manager));
			nodes_map[node_desc] = node;

			return node;
		}

		node_ptr Manager::createNode(const NodeDescription& node_desc, MyUser user) {
			if(nodes_map.find(node_desc) != nodes_map.end())
				throw NodeException("Node with such description already exists");

			node_ptr node(new Node(node_desc, connection_manager, user));
			nodes_map[node_desc] = node;
			return node;
		}

		node_ptr Manager::getNode(const NodeDescription& node_desc) const{
			auto node = nodes_map.find(node_desc);
			if(node == nodes_map.end())
			   	throw NodeException("No such node");

			return node->second;
		}

		node_ptr Manager::getNode(const std::string& node_name) const{
			for(auto node_pair: nodes_map) 
				if(node_pair.first.node_name == node_name) return node_pair.second;

			throw NodeException("No such node");
		}

		std::vector<node_ptr> Manager::getAllNodes() const{
			std::vector<node_ptr> nodes;
			for(auto node_pair: nodes_map)
				nodes.push_back(node_pair.second);

			return nodes;
		}

		void Manager::removeNode(const NodeDescription& node_desc) {
        	auto node = nodes_map.find(node_desc);
			if(node == nodes_map.end()) 
				throw NodeException("No such node");
			nodes_map.erase(node);
		}

		void Manager::removeNode(const std::string& node_name) {
			for(auto node_pair: nodes_map) 
				if(node_pair.first.node_name == node_name) {
                	nodes_map.erase(node_pair.first);
					break;
				}

			throw NodeException("No such node");
		}


		network_ptr Manager::createNetwork(const NetworkDescription& network_desc) {
			if(networks_map.find(network_desc) != networks_map.end())
				throw NetworkException("Network with such description already exists");

			network_ptr network(new Network(network_desc));
			networks_map[network_desc] = network;

			return network;
		}

		network_ptr Manager::getNetwork(const NetworkDescription& network_desc) const {
			auto network = networks_map.find(network_desc);
			if(network == networks_map.end())
			   	throw NetworkException("No such network");

			return network->second;
		}

		network_ptr Manager::getNetwork(const std::string& network_name) const {
			for(auto network_pair: networks_map) 
				if(network_pair.first.network_name == network_name) return network_pair.second;

			throw NodeException("No such network");
		}

		std::vector<network_ptr> Manager::getAllNetworks() const {
			std::vector<network_ptr> networks;
			for(auto network_pair: networks_map)
				networks.push_back(network_pair.second);

			return networks;
		}

		void Manager::removeNetwork(const NetworkDescription& network_desc) {
        	auto network = networks_map.find(network_desc);
			if(network == networks_map.end()) 
				throw NetworkException("No such network");
			networks_map.erase(network);
		}

		void Manager::removeNetwork(const std::string& network_name) {
			for(auto network_pair: networks_map) 
				if(network_pair.first.network_name == network_name) {
					networks_map.erase(network_pair.first);
					break;
				}

			throw NetworkException("No such network");
		}

		void Manager::setDataManager(DataManager& dataManager) {
			this->data_manager = dataManager;
		}

		void Manager::saveState() {
			std::shared_ptr<serialization::AbstractSink> sink = data_manager.getSink(); 

			try {
				std::string nodes_count(std::to_string(nodes_map.size()));
				*sink << nodes_count;
				for(auto node_pair: nodes_map) *sink << *node_pair.second;

				std::vector<types::NodeDescription> related_nodes;
				std::string network_count(std::to_string(networks_map.size()));
				*sink << network_count;

				for(auto network_pair: networks_map) {
					*sink << *network_pair.second;

					for(auto node_pair: network_pair.second->getAssociatedNodes()) 
						related_nodes.push_back(node_pair.first);

					*sink << related_nodes;
					related_nodes.clear();
				}
			} catch(serialization::DataException& de) {
				throw de;
			}
			catch(std::exception& e) {
				throw serialization::DataException("Could not save object");
			}
		}

		void Manager::saveState(const std::string& dest_file) {
			data_manager.setDataFile(dest_file);
			saveState();
		}

		void Manager::loadState() {
			clear();

			std::shared_ptr<serialization::AbstractSource> source = data_manager.getSource();

			try {
				std::string count;
				*source >> count;
				int nc = std::stoi(count);

				types::NodeDescription temp_node_desc;
				for(int i = 0; i < nc; ++i) {
					node_ptr node(new Node(temp_node_desc, connection_manager));
					*source >> *node;
					nodes_map[node->getDescription()] = node;
				}

				*source >> count;
				nc = std::stoi(count);
				types::NetworkDescription temp_network_desc;
				std::vector<types::NodeDescription> related_nodes;
				for(int i = 0; i < nc; i++) {

					network_ptr network(new Network(temp_network_desc));
					*source >> *network;
					networks_map[network->getDescription()] = network;

					*source >> related_nodes;
					for(auto& node_desc: related_nodes) {

						auto node_it = nodes_map.find(node_desc);
						if(node_it == nodes_map.end()) {
							throw serialization::DataException("Bad data format");
						} else {
							network->associateNode(node_it->second);
						}
					}
				}
			} catch(serialization::DataException& de) {
				throw de;
			} catch(std::exception& e) {
				throw serialization::DataException("Could not load object");
			}
		}

		void Manager::loadState(const std::string& source_file) {
			data_manager.setDataFile(source_file);
			loadState();
		}

		void Manager::clear() {
			nodes_map.clear();
			networks_map.clear();
		}


	} /* namespace sercli */
} /* namespace sp2p */
