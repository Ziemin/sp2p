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

#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include "user.hpp"
#include "nodeconnection.hpp"
#include "sp2p_types.hpp"
#include "logging.hpp"

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

                void  setUser(const MyUser& new_user);

                const MyUser& getUser() const;

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

                NodeError registerNetwork(const NetworkDescription& network_desc);
                NodeError deleteNetwork(const NetworkDescription& network_desc);

                std::tuple<NodeError, std::int32_t> updateServer(const NetworkDescription& network_desc, const std::int32_t port); 

                // async function - handler signature is updateHandler(NodeError, std::int32_t)
                template<typename UpdateHandler>
                    void asyncUpdateServer(const NetworkDescription& network_desc, const std::int32_t port, UpdateHandler updateHandler);

                NodeError stopServer(const NetworkDescription& network_desc); 

                std::tuple<NodeError, Botan::X509_Certificate*> signKey(const Botan::Public_Key& public_key, 
                        const NetworkDescription* network_desc);

                const NodeDescription& getDescription();
                void setNewDescription(NodeDescription node_desc);

                void stopConnections();

            private:

                NodeError beforeMessage();

            private:

                NodeDescription node_desc;
                MyUser my_user;
                NodeConnection node_connection;
                std::vector<Botan::X509_CA> ca_list;
                std::vector<Botan::X509_Certificate> user_certificates;

                logging::Logger lg = logging::sp2p_lg::get();

            // serialization
            private:
                friend class boost::serialization::access;

                template<class Archive> void save(Archive& ar, const unsigned int /* version */) const {
                    ar & node_desc;
                    ar & my_user;
                    // TODO Crypto layer
                }

                template<class Archive> void load(Archive& ar, const unsigned int /* version */) {
                    ar & node_desc;
                    ar & my_user;
                    // TODO Crypto layer
                }
                BOOST_SERIALIZATION_SPLIT_MEMBER();
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
