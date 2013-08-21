#include <thread>
#include <boost/asio.hpp>

#include "manager.hpp"
#include "globals.hpp"


namespace sp2p {
	namespace sercli {

		Manager::Manager() : io_s(*global::io_s) { }

		Manager::Manager(DataManager& dataManager) : data_manager(&dataManager), io_s(*global::io_s) { }

		void Manager::stopAll() {
			// TODO 
		}

		Node& Manager::createNode(NodeDescription node_desc) {
			if(nodes_map.find(node_desc) != nodes_map.end())
				throw NodeException("Node with such description already exists");

			node_ptr node(new Node(node_desc, connection_manager));
			nodes_map[node_desc] = node;

			return *node;
		}

		Node& Manager::createNode(NodeDescription node_desc, MyUser user) {
			if(nodes_map.find(node_desc) != nodes_map.end())
				throw NodeException("Node with such description already exists");

			node_ptr node(new Node(node_desc, connection_manager, user));
			nodes_map[node_desc] = node;
			return *node;
		}

		Node& Manager::getNode(NodeDescription node_desc) const{
			auto node = nodes_map.find(node_desc);
			if(node == nodes_map.end())
			   	throw NodeException("No such node");

			return *node->second;
		}

		Node& Manager::getNode(std::string node_name) const{
			for(auto node_pair: nodes_map) 
				if(node_pair.first.node_name == node_name) return *node_pair.second;

			throw NodeException("No such node");
		}

		std::vector<node_ptr> Manager::getAllNodes() const{
			std::vector<node_ptr> nodes;
			for(auto node_pair: nodes_map)
				nodes.push_back(node_pair.second);

			return nodes;
		}

		void Manager::removeNode(NodeDescription node_desc) {
        	auto node = nodes_map.find(node_desc);
			if(node == nodes_map.end()) 
				throw NodeException("No such node");
			nodes_map.erase(node);
		}

		void Manager::removeNode(std::string node_name) {
			for(auto node_pair: nodes_map) 
				if(node_pair.first.node_name == node_name) {
                	nodes_map.erase(node_pair.first);
					break;
				}

			throw NodeException("No such node");
		}


		Network& Manager::createNetwork(NetworkDescription network_desc) {
			if(networks_map.find(network_desc) != networks_map.end())
				throw NetworkException("Network with such description already exists");

			network_ptr network(new Network(network_desc));
			networks_map[network_desc] = network;

			return *network;
		}

		Network& Manager::getNetwork(NetworkDescription network_desc) const {
			auto network = networks_map.find(network_desc);
			if(network == networks_map.end())
			   	throw NetworkException("No such network");

			return *network->second;
		}

		Network& Manager::getNetwork(std::string network_name) const {
			for(auto network_pair: networks_map) 
				if(network_pair.first.network_name == network_name) return *network_pair.second;

			throw NodeException("No such network");
		}

		std::vector<network_ptr> Manager::getAllNetworks() const {
			std::vector<network_ptr> networks;
			for(auto network_pair: networks_map)
				networks.push_back(network_pair.second);

			return networks;
		}

		void Manager::removeNetwork(NetworkDescription network_desc) {
        	auto network = networks_map.find(network_desc);
			if(network == networks_map.end()) 
				throw NetworkException("No such network");
			networks_map.erase(network);
		}

		void Manager::removeNetwork(std::string network_name) {
			for(auto network_pair: networks_map) 
				if(network_pair.first.network_name == network_name) {
					networks_map.erase(network_pair.first);
					break;
				}

			throw NetworkException("No such network");
		}

		void Manager::setDataManager(DataManager& dataManager) {
			this->data_manager = &dataManager;
		}


	} /* namespace sercli */
} /* namespace sp2p */
