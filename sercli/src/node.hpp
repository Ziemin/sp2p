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
#include <unordered_map>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include "user.hpp"
#include "nodeconnection.hpp"
#include "sp2p_types.hpp"
#include "logging.hpp"
#include "globals.hpp"
#include "encryption.hpp"

using namespace sp2p::sercli::types;

namespace sp2p {
    namespace sercli {

        typedef std::unordered_map<types::NetworkDescription, enc::cert_st_ptr, types::NetworkHash> net_cert_map;
        typedef std::unordered_map<types::NetworkDescription, enc::priv_st_ptr, types::NetworkHash> net_key_map;
        // Error from node and nodeclass

        class Node : boost::noncopyable { 
            
            public:
                /**
                 * Constructor...
                 */
                Node(NodeDescription node_desc, ConnectionManager<NodeRequest, NodeResponse>&);

                Node(NodeDescription node_desc, ConnectionManager<NodeRequest, NodeResponse>&, MyUser user);

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


                bool isLogged() const;

                NodeError registerNetwork(const NetworkDescription& network_desc);
                NodeError deleteNetwork(const NetworkDescription& network_desc);

                NodeError inviteUser(const NetworkDescription& network_desc, const User& user);

                std::tuple<NodeError, std::int32_t> updateServer(const NetworkDescription& network_desc, const std::int32_t port); 

                // async function - handler signature is updateHandler(NodeError, std::int32_t)
                template<typename UpdateHandler>
                    void asyncUpdateServer(const NetworkDescription& network_desc, const std::int32_t port, UpdateHandler updateHandler);

                NodeError stopServer(const NetworkDescription& network_desc); 

                std::tuple<NodeError, Botan::X509_Certificate*> signKey(const Botan::Private_Key& public_key, 
                        const types::NetworkDescription* network_desc);

                const NodeDescription& getDescription();
                void setNewDescription(NodeDescription node_desc);

                void stopConnections();

                std::vector<enc::priv_st_ptr>& getMyKeys();
                std::vector<enc::cert_st_ptr>& getNodeCerts();
                net_cert_map& getNetworkCerts();
                net_key_map& getNetworkKeys();
                std::vector<enc::cert_st_ptr>& getFreeCerts();

            private:

                NodeError beforeMessage();

            private:

                NodeDescription node_desc;
                MyUser my_user;

                std::vector<enc::priv_st_ptr> private_keys;                                 // private keys
                std::vector<enc::cert_st_ptr> free_certs;                                   // not related to network certificates
                net_cert_map net_certs;                                                     // certificates for particular networks
                net_key_map net_keys;                                                       // private keys for particular networks
                std::vector<enc::cert_st_ptr> node_certs;                                   // node certificates

                NodeConnection node_connection;

                logging::Logger& lg = logging::sp2p_lg::get();

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
