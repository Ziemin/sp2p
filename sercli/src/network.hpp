#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <map>
#include <memory>
#include <boost/asio.hpp>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>

#include "user.hpp"
#include "node.hpp"
#include "sp2p_types.hpp"


namespace sp2p {
    namespace sercli {

        class Node;
        /*
         * Class managing network existing on some nodes. It is assumed, that
         * such network is accessible to user on all of the nodes
         */
        class Network : boost::noncopyable {

            public:

                Network(types::NetworkDescription network_desc);
                /**
                 * Associates node with certain network
                 * @param network_desc data identyfying network created on node
                 */
                void associateNode(std::weak_ptr<Node> node_ptr);

                /**
                 * Removes node from network
                 * @param network_id data identyfying network created on node
                 */
                void detachNode(const types::NodeDescription& node_desc);

                bool isActive();

                const types::NetworkDescription& getDescription() const;
                const std::map<types::NodeDescription, std::weak_ptr<Node>>& getAssociatedNodes() const;

                std::vector<types::ServerDescription> getAvailableServers() const;

            private:

                void clearExpiredNodes();

            private:

                types::NetworkDescription network_desc;
                std::map<types::NodeDescription, std::weak_ptr<Node>> node_set;

            // serialization
            private:
                friend class boost::serialization::access;

                template<class Archive> void save(Archive& ar, const unsigned int /* version */) const {
                    ar & network_desc;
                }

                template<class Archive> void load(Archive& ar, const unsigned int /* version */) {
                    ar & network_desc;
                }
                BOOST_SERIALIZATION_SPLIT_MEMBER();
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
