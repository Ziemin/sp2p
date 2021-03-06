#include "node.hpp"

#include <botan/x509self.h>
#include <botan/dsa.h>
#include <botan/rsa.h>
#include <botan/data_src.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include "message_factory.hpp"

using namespace sp2p::sercli::types;
using namespace boost::log::trivial;

namespace sp2p {
    namespace sercli {

        std::ostream& operator<<(std::ostream& os, const protocol::NodeMessage::ResponseType& resp_type) {
            switch(resp_type) {
                case protocol::NodeMessage::OK:
                    os << "OK";
                    break;
                case protocol::NodeMessage::INTERNAL_SERVER_ERROR:
                    os << "INTERNAL SERVER ERROR";
                    break;
                case protocol::NodeMessage::BAD_REQUEST:
                    os << "BAD REQUEST";
                    break;
                case protocol::NodeMessage::NO_PRIVILAGES:
                    os << "NO PRIVILAGES";
                    break;
                case protocol::NodeMessage::NOT_LOGGED:
                    os << "NOT LOGGED";
                    break;
                case protocol::NodeMessage::BAD_CREDENTIALS:
                    os << "BAD CREDENTIALS";
                    break;
                case protocol::NodeMessage::NO_SUCH_NETWORK:
                    os << "NO SUCH NETWORK";
                    break;
                case protocol::NodeMessage::NO_SUCH_USER:
                    os << "NO SUCH USER";
                    break;
                case protocol::NodeMessage::BAD_DATA:
                    os << "BAD DATA";
                    break;
                case protocol::NodeMessage::ALREADY_EXISTS:
                    os << "ALREADY EXISTS";
                    break;
            }
            return os;
        }

        // --------- Node --------------------------

        Node::Node(NodeDescription node_desc, 
                ConnectionManager<NodeRequest, NodeResponse>& connection_manager) 
            : node_desc(node_desc),
            my_user("user", "password", "email"),
            node_connection(connection_manager, node_certs)
        {
        }

        // Node
        Node::Node(NodeDescription node_desc, 
                ConnectionManager<NodeRequest, NodeResponse>& connection_manager, 
                MyUser user)
            : node_desc(node_desc), 
            my_user(user), 
            node_connection(connection_manager, node_certs)
        { 
        }

        void Node::setUser(const MyUser& user) {
            if(isActive()) throw NodeException("Node is active at the moment");
            my_user = user;
            node_connection.setCookie("");
            node_connection.is_logged = false;
            free_certs.clear();
            net_certs.clear();
            private_keys.clear();
        }

        const MyUser& Node::getUser() const {
            return my_user; 
        }

        bool Node::isActive() const {
            return node_connection.isActive();
        }

        bool Node::isRegistered() const {
            return this->my_user.is_registered;
        }

        bool Node::isLogged() const {
            return node_connection.is_logged;
        }

        void Node::setNewDescription(NodeDescription node_desc) {
            if(this->isActive()) {
                BOOST_LOG_SEV(lg, error) << "Setting new description to active node";
                throw NodeException("Node is active at the moment");
            }

            this->node_desc = node_desc;
        }

        const NodeDescription& Node::getDescription() {
            return node_desc;
        }

        NodeError Node::beforeMessage() {
            if(!node_connection.is_logged) 
                return NodeError::NOT_LOGGED;

            try {
                if(!isActive()) node_connection.connect(node_desc);  
                else node_connection.resetDeadlineTimer(global::node_timeout_seconds);
                return NodeError::OK;
            } catch(NodeError& error) {
                return error;
            }
        }

        NodeError Node::logIn() {

            BOOST_LOG_SEV(lg, trace) << "Logging to " << node_desc;
            NodeError result = NodeError::OK;

            if(!node_connection.is_logged) {
                try {
                    if(!isActive()) node_connection.connect(node_desc);
                    else node_connection.resetDeadlineTimer(global::node_timeout_seconds);
                } catch(NodeError& error) {
                    return error;
                }

                std::shared_ptr<NodeRequest> loginMessage = 
                    utils::getLoginMessage(my_user.username, my_user.password);

                BOOST_LOG_SEV(lg, debug) << "Prepared message for " << node_desc;
                connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
                try {
                    std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(loginMessage).get();
                    protocol::NodeMessage& response = nodeResponse->getResponse();

                    BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                        " -> " << response.response_type();
                    switch(response.response_type()) {

                        case protocol::NodeMessage::OK:
                            if(response.has_login_response()) {
                                const protocol::NodeMessage::Login& loginResponse = response.login_response();
                                if(loginResponse.has_cookie()) {
                                    result = NodeError::OK;
                                    BOOST_LOG_SEV(lg, info) << "Cookie from: " << node_desc << " = " << loginResponse.cookie();
                                    node_connection.setCookie(loginResponse.cookie());
                                    node_connection.is_logged = true;

                                    if(!my_user.is_registered) my_user.is_registered = true;

                                    break;
                                }
                            } 
                            result = NodeError::BAD_SERVER_RESPONSE;
                            break;

                        default:
                            result = utils::getDefaultError(response.response_type());
                    }

                    node_connection.resetDeadlineTimer(global::node_timeout_seconds);

                } catch(SendException& e) {
                    return NodeError::SEND_ERROR;
                }
            }

            return result;
        }


        NodeError Node::logOut() {

            BOOST_LOG_SEV(lg, trace) << "Logging out from " << node_desc;
            NodeError result = beforeMessage();
            if(any(result)) return result;

            std::shared_ptr<NodeRequest> logoutMessage = utils::getLogoutMessage(node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(logoutMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        node_connection.is_logged = false;
                        node_connection.setCookie("");
                        result = NodeError::OK;

                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                        result = NodeError::OK;
                        break;

                    default:
                        result = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                return NodeError::SEND_ERROR;
            }

            return result;
        }

        NodeError Node::registerUser() {
            if(my_user.is_registered) return NodeError::OK;

            BOOST_LOG_SEV(lg, trace) << "Registering user to " << node_desc;

            NodeError result;
            try {
                if(!isActive()) node_connection.connect(node_desc);
                else node_connection.resetDeadlineTimer(global::node_timeout_seconds);
            } catch(NodeError& error) {
                return error;
            }

            
            // creating sign request 
            Botan::AutoSeeded_RNG rng;
            
            Botan::X509_Cert_Options opts;
            opts.common_name = my_user.username;
            opts.email = my_user.email;
            opts.organization = node_desc.node_name;
            opts.country = "Pl";

            if(private_keys.empty()) {
                BOOST_LOG_SEV(lg, error) << "No private key available for registration";
                return NodeError::OTHER;  
            }

            Botan::Private_Key* priv_key = private_keys[0]->getKey();
            if(priv_key == nullptr) {
                BOOST_LOG_SEV(lg, debug) << "Private key is null";
                return NodeError::OTHER;
            } 

            Botan::PKCS10_Request request = Botan::X509::create_cert_req(opts, *priv_key, "SHA-256", rng);

            std::shared_ptr<NodeRequest> registerUserMessage = 
                utils::getRegisterUserMessage(my_user, request.PEM_encode()); 

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(registerUserMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        {
                            my_user.is_registered = true;

                            // getting node cert and my cert
                            const protocol::NodeMessage::Register& reg_resp = response.register_response();
                            const std::string& my_cert = reg_resp.user_certificate();
                            const std::string& node_cert = reg_resp.node_certificate();

                            BOOST_LOG_SEV(lg, debug) << "Node cert from node: " << node_desc << " -> "
                                << node_cert;
                            BOOST_LOG_SEV(lg, debug) << "My cert from node: " << node_desc << " -> "
                                << my_cert;

                            Botan::DataSource_Memory ds_my(my_cert);
                            Botan::DataSource_Memory ds_node(node_cert);

                            try {
                                enc::cert_st_ptr cert_st_my(new enc::CertificateStore(new Botan::X509_Certificate(ds_my)));
                                cert_st_my->setName(node_desc.node_name+"."+std::to_string(free_certs.size())+".pem");
                                cert_st_my->setFilename(node_desc.node_name+"."+std::to_string(free_certs.size())+".pem");

                                enc::cert_st_ptr cert_st_node(new enc::CertificateStore(new Botan::X509_Certificate(ds_node)));
                                cert_st_node->setName(node_desc.node_name+"."+std::to_string(node_certs.size())+".pem");
                                cert_st_node->setFilename(node_desc.node_name+"."+std::to_string(node_certs.size())+".pem");


                                node_certs.push_back(cert_st_node);
                                free_certs.push_back(cert_st_my);

                                result = NodeError::OK;
                            } catch(std::exception& e) {
                                BOOST_LOG_SEV(lg, error) << e.what();
                                result = NodeError::BAD_SERVER_RESPONSE;
                            }
                            break;
                        }

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        result = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                return NodeError::SEND_ERROR;
            }
            return result;
        }

        NodeError Node::changePassword(const std::string& new_password) {
            BOOST_LOG_SEV(lg, trace) << "Changing password on " << node_desc;

            NodeError result = beforeMessage();
            if(any(result)) return result;

            std::shared_ptr<NodeRequest> changePasswordMessage = 
                utils::getChangePasswordMessage(my_user, new_password, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(changePasswordMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        my_user.password = new_password;
                        result = NodeError::OK;
                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        result = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                return NodeError::SEND_ERROR;
            }

            return result;
        }


        std::tuple<NodeError, User> Node::getUserInfo(const NetworkDescription& network_desc, const std::string& username) {

            BOOST_LOG_SEV(lg, trace) << "Getting user info from " << node_desc;
            std::tuple<NodeError, User> resultTuple;

            NodeError error = beforeMessage();
            if(any(error)) {
                std::get<0>(resultTuple) = error;
                return resultTuple;
            }

            std::shared_ptr<NodeRequest> userInfoMessage = 
                utils::getUserInfoMessage(network_desc, username, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(userInfoMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        std::get<1>(resultTuple) = { username, "" }; //TODO public key
                        error = NodeError::OK;
                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }
            std::get<0>(resultTuple) = error;
            return resultTuple;
        }

        NodeError Node::inviteUser(const NetworkDescription& network_desc, const User& user) {

            BOOST_LOG_SEV(lg, trace) << "Inviting user to " << node_desc << " -> " << user;
            NodeError error = beforeMessage();
            if(any(error)) return error;

            std::shared_ptr<NodeRequest> inviteUserMessage = 
                utils::getInviteUserMessage(network_desc, user, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(inviteUserMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        error = NodeError::OK;
                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            return error;
        }
        
        std::tuple<NodeError, std::vector<NetworkDescription>> Node::getNetworksList() {

            BOOST_LOG_SEV(lg, trace) << "Getting network list from " << node_desc;
            std::tuple<NodeError, std::vector<NetworkDescription>> resultTuple;

            NodeError error = beforeMessage();
            if(any(error)) {
                std::get<0>(resultTuple) = error;
                return resultTuple;
            }

            std::shared_ptr<NodeRequest> listNetworksMessage = 
                utils::getListNetworksMessage(node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(listNetworksMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        {
                            std::vector<NetworkDescription>& networks = std::get<1>(resultTuple);

                            const protocol::NodeMessage::ListNetworks& netList = response.list_networks_response();
                            for(int i = 0; i < netList.network_list_size(); i++) {
                                const protocol::NodeMessage::ListNetworks::Network& nodeNetwork = netList.network_list(i);
                                networks.push_back(utils::getNetworkDescriptionFromProto(nodeNetwork));
                            }

                            error = NodeError::OK;
                            break;
                        }

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            std::get<0>(resultTuple) = error;
            return resultTuple;
        }

        std::tuple<NodeError, std::vector<NetworkDescription>> Node::getMyNetworks() {

            BOOST_LOG_SEV(lg, trace) << "Getting my networks from " << node_desc;
            std::tuple<NodeError, std::vector<NetworkDescription>> resultTuple;

            NodeError error = beforeMessage();
            if(any(error)) {
                std::get<0>(resultTuple) = error;
                return resultTuple;
            }

            std::shared_ptr<NodeRequest> listMyNetworksMessage = 
                utils::getListMyNetworksMessage(node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(listMyNetworksMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        {
                            std::vector<NetworkDescription>& networks = std::get<1>(resultTuple);

                            const protocol::NodeMessage::ListMyNetworks& netList = response.list_my_networks_response();
                            for(int i = 0; i < netList.network_list_size(); i++) {
                                const protocol::NodeMessage::ListNetworks::Network& nodeNetwork = netList.network_list(i);
                                networks.push_back(utils::getNetworkDescriptionFromProto(nodeNetwork));
                            }

                            error = NodeError::OK;
                            break;
                        }

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            std::get<0>(resultTuple) = error;
            return resultTuple;
        }

        std::tuple<NodeError, std::vector<ServerDescription>> Node::getServersList(const NetworkDescription& network_desc) {

            BOOST_LOG_SEV(lg, trace) << "Getting server list from " << node_desc << " -> " << network_desc;
            std::tuple<NodeError, std::vector<ServerDescription>> resultTuple;

            NodeError error = beforeMessage();
            if(any(error)) {
                std::get<0>(resultTuple) = error;
                return resultTuple;
            }

            std::shared_ptr<NodeRequest> listServersMessage = 
                utils::getListServersMessage(network_desc, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(listServersMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        {
                            std::vector<ServerDescription>& servers = std::get<1>(resultTuple);

                            const protocol::NodeMessage::ListServers& serverList = response.list_servers_response();
                            for(int i = 0; i < serverList.list_servers_size(); i++) {
                                const protocol::NodeMessage::ListServers::Server& nodeServer = serverList.list_servers(i);
                                servers.push_back(utils::getServerDescriptionFromProto(nodeServer, network_desc));
                            }

                            error = NodeError::OK;
                            break;
                        }

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            std::get<0>(resultTuple) = error;
            return resultTuple;
        }

        NodeError Node::registerNetwork(const NetworkDescription& network_desc) {

            BOOST_LOG_SEV(lg, trace) << "Registering network on " << node_desc << " -> " << network_desc;
            NodeError error = beforeMessage();
            if(any(error)) return error;

            std::shared_ptr<NodeRequest> createNetworkMessage = 
                utils::getCreateNetworkMessage(network_desc, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(createNetworkMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        error = NodeError::OK;
                        break;
                        
                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            return error;
        }

        NodeError Node::deleteNetwork(const NetworkDescription& network_desc) {

            BOOST_LOG_SEV(lg, trace) << "Removing network from " << node_desc << " -> " << network_desc;
            NodeError error = beforeMessage();
            if(any(error)) return error;

            std::shared_ptr<NodeRequest> unregisterNetworkMessage = 
                utils::getDeleteNetworkMessage(network_desc, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(unregisterNetworkMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        error = NodeError::OK;
                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            return error;
        }

        // ------- Update Server ---------------------------------------------------------------------------------------

        std::tuple<NodeError, std::int32_t> Node::updateServer(const NetworkDescription& network_desc,
               const std::int32_t port) {

            BOOST_LOG_SEV(lg, trace) << "Updating server on " << node_desc << " and port " << port;
            std::tuple<NodeError, std::int32_t> resultTuple;
            NodeError error = beforeMessage();
            if(any(error)) {
                std::get<0>(resultTuple) = error;
                return resultTuple;
            }

            std::shared_ptr<NodeRequest> updateServerMessage = 
                utils::getUpdateServerMessage(network_desc, port, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(updateServerMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        error = NodeError::OK;
                        std::get<1>(resultTuple) = response.update_server_response().seconds_to_update();
                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }
            std::get<0>(resultTuple) = error;

            return resultTuple;
        }

        template<typename UpdateHandler>
            void Node::asyncUpdateServer(const NetworkDescription& network_desc, const std::int32_t port, UpdateHandler updateHandler) {
                // TODO implementation change completely to be more asynchronous

                BOOST_LOG_SEV(lg, trace) << "Async updating server on " << node_desc << " - > " 
                    <<  network_desc << " and port " << port;
                NodeError error = beforeMessage();
                if(any(error)) {
                    updateHandler(error, 0);
                    return;
                }

                std::shared_ptr<NodeRequest> updateServerMessage = 
                    utils::getUpdateServerMessage(network_desc, port, node_connection.getCookie());

                connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
                NodeConnection* nc = &node_connection;
                con->sendMessage(updateServerMessage, 
                        [this, con, updateHandler, nc](boost::system::error_code ec) 
                        {
                            if(!ec) {
                                con->readMessage([&updateHandler](std::shared_ptr<NodeResponse> response_ptr,
                                        boost::system::error_code ec)  
                                {

                                    NodeError error;
                                    protocol::NodeMessage response = response_ptr->getResponse();

                                    switch(response.response_type()) {
                                        case protocol::NodeMessage::OK:
                                            updateHandler(NodeError::OK, response.update_server_response().seconds_to_update());
                                            break;

                                        case protocol::NodeMessage::NOT_LOGGED:
                                            nc->is_logged = false;
                                        default:
                                            error = utils::getDefaultError(response.response_type());
                                            updateHandler(error, 0);
                                    }
                                });
                            } else {
                                updateHandler(NodeError::SEND_ERROR, 0);
                            }
                        });
            }

        // ------- \ Update Server ---------------------------------------------------------------------------------------


        NodeError Node::stopServer(const NetworkDescription& network_desc) {

            BOOST_LOG_SEV(lg, trace) << "Stopping server on on " << node_desc << " -> " << network_desc;
            NodeError error = beforeMessage();
            if(any(error)) return error;

            std::shared_ptr<NodeRequest> stopServerMessage = 
                utils::getStopServerMessage(network_desc, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(stopServerMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        error = NodeError::OK;
                        break;

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        error = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                error = NodeError::SEND_ERROR;
            }

            return error;
        }

        std::tuple<NodeError, Botan::X509_Certificate*> Node::signKey(const Botan::Private_Key& priv_key, 
                const types::NetworkDescription* network_desc) {

            BOOST_LOG_SEV(lg, trace) << "Signing key on " << node_desc;
            if(network_desc != nullptr) BOOST_LOG_SEV(lg, trace) << " - > " << *network_desc;

            std::tuple<NodeError, Botan::X509_Certificate*> resultTuple;
            NodeError res = beforeMessage();
            if(any(res)) {
                std::get<0>(resultTuple) = res;
                return resultTuple;
            }
            // creating sign request 
            if(private_keys.empty()){
                std::get<0>(resultTuple) = NodeError::OTHER;
                return resultTuple;
            } 

            Botan::AutoSeeded_RNG rng;
            
            Botan::X509_Cert_Options opts;
            opts.common_name = my_user.username;
            opts.email = my_user.email;
            opts.organization = node_desc.node_name;
            opts.org_unit = network_desc->network_name;
            opts.country = "Pl";

            Botan::PKCS10_Request request = Botan::X509::create_cert_req(opts, priv_key, "SHA-256", rng);

            std::shared_ptr<NodeRequest> signKeyMessage = 
                utils::getSignKeyMessage(request.PEM_encode(), network_desc, node_connection.getCookie());

            connection_ptr<NodeRequest, NodeResponse> con = node_connection.getConnection();
            try {
                std::shared_ptr<NodeResponse> nodeResponse = con->sendRequest(signKeyMessage).get();
                protocol::NodeMessage response = nodeResponse->getResponse();

                BOOST_LOG_SEV(lg, info) << "Response from " << node_desc <<
                    " -> " << response.response_type();
                switch(response.response_type()) {
                    case protocol::NodeMessage::OK:
                        {
                            Botan::DataSource_Memory data_source(response.sign_key_response().user_certificate());
                            try {
                                Botan::X509_Certificate* certificate = new Botan::X509_Certificate(data_source);

                                if(network_desc != nullptr) {
                                    enc::cert_st_ptr cert_s = std::make_shared<enc::CertificateStore>(certificate);
                                    cert_s->setName(node_desc.node_name + "." + network_desc->network_name);
                                    cert_s->setFilename(node_desc.node_name + "." + network_desc->network_name + ".pem");
                                    net_certs[*network_desc] = cert_s;
                                }

                                res = NodeError::OK;
                                std::get<1>(resultTuple) = certificate;
                            } catch(std::exception& e) {
                                BOOST_LOG_SEV(lg, error) << e.what();
                                res = NodeError::BAD_SERVER_RESPONSE;
                            }
                            break;
                        }

                    case protocol::NodeMessage::NOT_LOGGED:
                        node_connection.is_logged = false;
                    default:
                        res = utils::getDefaultError(response.response_type());
                }

                node_connection.resetDeadlineTimer(global::node_timeout_seconds);

            } catch(SendException& e) {
                res = NodeError::SEND_ERROR;
            }

            std::get<0>(resultTuple) = res;
            return resultTuple;
        }

        void Node::stopConnections() {
            BOOST_LOG_SEV(lg, trace) << "Stopping all connections";
            if(isActive()) node_connection.disconnect();
        }

        std::vector<enc::priv_st_ptr>& Node::getMyKeys() {
            return private_keys;
        }
        std::vector<enc::cert_st_ptr>& Node::getNodeCerts() {
            return node_certs;
        }
        net_cert_map& Node::getNetworkCerts() {
            return net_certs;
        }

        net_key_map& Node::getNetworkKeys() {
            return net_keys;
        }
        std::vector<enc::cert_st_ptr>& Node::getFreeCerts() {
            return free_certs;
        }

    } /* namespace sercli */

} /* namespace sp2p */
