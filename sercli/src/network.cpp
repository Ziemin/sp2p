#include "network.hpp"

namespace sp2p {
    namespace sercli {

        Network::Network(types::NetworkDescription network_desc) {
            this->network_desc = network_desc;
        }

        void Network::associateNode(std::weak_ptr<Node> node_ptr) {
            node_set.insert(std::make_pair(node_ptr.lock()->getDescription(), node_ptr));
        }

        void Network::detachNode(const types::NodeDescription& node_desc) {
            node_set.erase(node_desc);
        }

        const types::NetworkDescription& Network::getDescription() const {
            return network_desc;
        }

        const std::map<NodeDescription, std::weak_ptr<Node>>& Network::getAssociatedNodes() const {
            return node_set; 
        }

        void Network::clearExpiredNodes() {

            auto it = node_set.begin();
            while(it != node_set.end()) {
                if(it->second.expired()) node_set.erase(it++);
                else ++it;
            }
        }

        bool Network::isActive() {

            bool active = false;

            auto it = node_set.begin();
            while(it != node_set.end()) {
                if(it->second.expired()) node_set.erase(it++);
                else {
                    active |= it->second.lock()->isActive();
                    ++it;
                }
            }

            return active;
        }

        // Synchronous operations for a while
        std::vector<types::ServerDescription> Network::getAvailableServers() const {

            std::vector<types::ServerDescription> availServers;

            for(auto node_it: node_set) {
                node_ptr node = node_it.second.lock();
                std::tuple<NodeError, std::vector<ServerDescription>> query_result = node->getServersList(network_desc);
                if(std::get<0>(query_result) != NodeError::OK) {
                    // log error
                    continue;
                } else {
                    availServers.insert(
                            availServers.end(), std::get<1>(query_result).begin(), std::get<1>(query_result).end());
                }
            }

            return availServers;
        }


    } /* namespace sercli */
} /* namespace sp2p */

