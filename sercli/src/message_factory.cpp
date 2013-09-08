#include "message_factory.hpp"

#include <boost/asio.hpp>
#include <botan/botan.h>
#include <botan/x509self.h>
#include <botan/dsa.h>
#include <botan/rsa.h>


using namespace sp2p::sercli::types;

namespace sp2p {
    namespace sercli {

        namespace utils {

            std::shared_ptr<NodeRequest> getLoginMessage(const std::string& username, const std::string& password) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage_RequestType_LOGIN);

                protocol::ClientMessage::Login *login_message = new protocol::ClientMessage::Login();
                login_message->set_allocated_username(new std::string(username));
                login_message->set_allocated_password(new std::string(password));

                protoRequest.set_allocated_login_message(login_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getLogoutMessage(const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::LOGOUT);

                protocol::ClientMessage::Logout *logout_message = new protocol::ClientMessage::Logout();
                logout_message->set_allocated_cookie(new std::string(cookie));

                protoRequest.set_allocated_logout_message(logout_message);

                return request;

            }

            std::shared_ptr<NodeRequest> getRegisterUserMessage(const MyUser& my_user, const std::string& public_key) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::REGISTER);

                protocol::ClientMessage::Register *register_message = new protocol::ClientMessage::Register();
                register_message->set_allocated_username(new std::string(my_user.username));
                register_message->set_allocated_password(new std::string(my_user.password));
                register_message->set_allocated_public_key(new std::string(public_key));
                
                protoRequest.set_allocated_register_message(register_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getUnregisterUserMessage(const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::UNREGISTER);

                protocol::ClientMessage::Unregister *unregister_message = new protocol::ClientMessage::Unregister();
                unregister_message->set_allocated_cookie(new std::string(cookie));
                
                protoRequest.set_allocated_unregister_message(unregister_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getChangePasswordMessage(const MyUser& my_user, 
                    const std::string& new_password, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::CHANGE_PASSWORD);

                protocol::ClientMessage::ChangePassword *change_password_message = new protocol::ClientMessage::ChangePassword();
                change_password_message->set_allocated_old_password(new std::string(my_user.password));
                change_password_message->set_allocated_new_password(new std::string(new_password));
                change_password_message->set_allocated_cookie(new std::string(cookie));
                
                protoRequest.set_allocated_change_password_message(change_password_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getUserInfoMessage(const NetworkDescription& network_desc, 
                        const std::string& username, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::USER_INFO);

                protocol::ClientMessage::UserInfo *user_info_message = new protocol::ClientMessage::UserInfo();
                user_info_message->set_allocated_username(new std::string(username));
                user_info_message->set_allocated_network_name(new std::string(network_desc.network_name));
                user_info_message->set_allocated_cookie(new std::string(cookie));
                
                protoRequest.set_allocated_user_info_message(user_info_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getListNetworksMessage(const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::LIST_NETWORKS);

                protocol::ClientMessage::ListNetworks *list_networks_message = new protocol::ClientMessage::ListNetworks();
                list_networks_message->set_allocated_cookie(new std::string(cookie));
                
                protoRequest.set_allocated_list_networks_message(list_networks_message);

                return request;
            }                   

            std::shared_ptr<NodeRequest> getListMyNetworksMessage(const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::LIST_MY_NETWORKS);

                protocol::ClientMessage::ListMyNetworks *list_my_networks_message = new protocol::ClientMessage::ListMyNetworks();
                list_my_networks_message->set_allocated_cookie(new std::string(cookie));
                
                protoRequest.set_allocated_list_my_networks_message(list_my_networks_message);

                return request;
            }                   

            std::shared_ptr<NodeRequest> getListServersMessage(const NetworkDescription& network_desc, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::LIST_SERVERS);

                protocol::ClientMessage::ListServers *list_servers_message = new protocol::ClientMessage::ListServers();
                list_servers_message->set_allocated_cookie(new std::string(cookie));
                list_servers_message->set_allocated_network_name(new std::string(network_desc.network_name));
                
                protoRequest.set_allocated_list_servers_message(list_servers_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getCreateNetworkMessage(const NetworkDescription& network_desc, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::CREATE_NETWORK);

                protocol::ClientMessage::CreateNetwork *create_network_message = new protocol::ClientMessage::CreateNetwork();
                create_network_message->set_allocated_cookie(new std::string(cookie));
                create_network_message->set_allocated_creator_email(new std::string(network_desc.creator.email));
                create_network_message->set_allocated_network_name(new std::string(network_desc.network_name));
                create_network_message->set_allocated_protocol_name(new std::string(network_desc.protocol_name));

                switch(network_desc.access_rights) {
                    case NetworkDescription::AccessRights::PRIVATE:
                        create_network_message->set_access_rights(protocol::ClientMessage::CreateNetwork::PRIVATE);
                        break;
                    case NetworkDescription::AccessRights::PUBLIC:
                        create_network_message->set_access_rights(protocol::ClientMessage::CreateNetwork::PUBLIC);
                        break;
                }
                switch(network_desc.participation_rights) {
                    case NetworkDescription::ParticipationRights::CLIENT_ONLY:
                        create_network_message->set_participation_rights(protocol::ClientMessage::CreateNetwork::CLIENT_ONLY);
                        break;
                    case NetworkDescription::ParticipationRights::CLIENT_SERVER:
                        create_network_message->set_participation_rights(protocol::ClientMessage::CreateNetwork::CLIENT_SERVER);
                        break;
                }
                switch(network_desc.visability) {
                    case NetworkDescription::Visibility::LISTED:
                        create_network_message->set_visibility(protocol::ClientMessage::CreateNetwork::LISTED);
                        break;
                    case NetworkDescription::Visibility::UNLISTED:
                        create_network_message->set_visibility(protocol::ClientMessage::CreateNetwork::UNLISTED);
                        break;
                }

                protoRequest.set_allocated_create_network_message(create_network_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getDeleteNetworkMessage(const NetworkDescription& network_desc, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::DELETE_NETWORK);

                protocol::ClientMessage::DeleteNetwork *delete_network_message = new protocol::ClientMessage::DeleteNetwork();
                delete_network_message->set_allocated_cookie(new std::string(cookie));
                delete_network_message->set_allocated_name(new std::string(network_desc.network_name));

                protoRequest.set_allocated_delete_network_message(delete_network_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getInviteUserMessage(const NetworkDescription& network_desc, const User& user,
                    const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::INVITE_USER);

                protocol::ClientMessage::InviteUser *invite_user_message = new protocol::ClientMessage::InviteUser();
                invite_user_message->set_allocated_network_name(new std::string(network_desc.network_name));
                invite_user_message->set_allocated_username(new std::string(user.username));
                invite_user_message->set_allocated_cookie(new std::string(cookie));

                protoRequest.set_allocated_invite_user_message(invite_user_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getRemoveUserMessage(const NetworkDescription& network_desc, const User& user,
                    const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::REMOVE_USER);

                protocol::ClientMessage::RemoveUser *remove_user_message = new protocol::ClientMessage::RemoveUser();
                remove_user_message->set_allocated_network_name(new std::string(network_desc.network_name));
                remove_user_message->set_allocated_username(new std::string(user.username));
                remove_user_message->set_allocated_cookie(new std::string(cookie));

                protoRequest.set_allocated_remove_user_message(remove_user_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getUpdateServerMessage(const NetworkDescription& network_desc, 
                    const std::int32_t port, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::UPDATE_SERVER);

                protocol::ClientMessage::UpdateServer *update_server_message = new protocol::ClientMessage::UpdateServer();
                update_server_message->set_allocated_network_name(new std::string(network_desc.network_name));
                update_server_message->set_allocated_cookie(new std::string(cookie));
                update_server_message->set_port_number(port);

                protoRequest.set_allocated_update_server_message(update_server_message);

                return request;

            }

            std::shared_ptr<NodeRequest> getStopServerMessage(const NetworkDescription& network_desc, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::STOP_SERVER);

                protocol::ClientMessage::StopServer *stop_server_message = new protocol::ClientMessage::StopServer();
                stop_server_message->set_allocated_network_name(new std::string(network_desc.network_name));
                stop_server_message->set_allocated_cookie(new std::string(cookie));

                protoRequest.set_allocated_stop_server_message(stop_server_message);

                return request;
            }

            std::shared_ptr<NodeRequest> getSignKeyMessage(const Botan::Public_Key& public_key,
                   const NetworkDescription* network_desc, const std::string& cookie) {

                std::shared_ptr<NodeRequest> request(new NodeRequest());
                protocol::ClientMessage& protoRequest = request->getRequest();
                protoRequest.set_request(protocol::ClientMessage::SIGN_KEY);

                protocol::ClientMessage::SignKey *sign_key_message = new protocol::ClientMessage::SignKey();
                sign_key_message->set_allocated_cookie(new std::string(cookie));
                sign_key_message->set_allocated_public_key(new std::string(Botan::X509::PEM_encode(public_key)));

                if(network_desc != nullptr)
                    sign_key_message->set_allocated_network_name(new std::string(network_desc->network_name));

                protoRequest.set_allocated_sign_key_message(sign_key_message);

                return request;
            }

            NetworkDescription getNetworkDescriptionFromProto(const protocol::NodeMessage::ListNetworks::Network& network) {

                NetworkDescription network_desc;

                network_desc.network_name = network.name();
                network_desc.creator = { network.creator_name(), network.creator_name() };
                network_desc.protocol_name = network.protocol_name();

                switch(network.participation_rights()) {
                    case protocol::ClientMessage::CreateNetwork::CLIENT_ONLY:
                        network_desc.participation_rights = NetworkDescription::ParticipationRights::CLIENT_ONLY;
                        break;
                    case protocol::ClientMessage::CreateNetwork::CLIENT_SERVER:
                        network_desc.participation_rights = NetworkDescription::ParticipationRights::CLIENT_SERVER;
                        break;
                }

                switch(network.access_rights()) {
                    case protocol::ClientMessage::CreateNetwork::PUBLIC:
                        network_desc.access_rights = NetworkDescription::AccessRights::PUBLIC;
                        break;
                    case protocol::ClientMessage::CreateNetwork::PRIVATE:
                        network_desc.access_rights = NetworkDescription::AccessRights::PRIVATE;
                        break;
                }


                return network_desc;
            }

            ServerDescription getServerDescriptionFromProto(const protocol::NodeMessage::ListServers::Server& server, 
                    const NetworkDescription& network_desc) {

                return {  // ServerDescription
                    server.username(), 
                    network_desc.network_name,
                    boost::asio::ip::address::from_string(server.ip_address().c_str())
                };

            }

            NodeError getDefaultError(protocol::NodeMessage::ResponseType responseCode) {

                switch(responseCode) {

                    case protocol::NodeMessage::OK:
                        return NodeError::OK;

                    case protocol::NodeMessage::BAD_CREDENTIALS:
                        return NodeError::BAD_CREDENTIALS;
                        
                    case protocol::NodeMessage::BAD_REQUEST:
                        return NodeError::BAD_REQUEST;
                        
                    case protocol::NodeMessage::NO_PRIVILAGES:
                        return NodeError::NO_PRIVILAGES;
                        
                    case protocol::NodeMessage::NO_SUCH_USER:
                        return NodeError::NO_SUCH_USER;
                        
                    case protocol::NodeMessage::NO_SUCH_NETWORK:
                        return NodeError::NO_SUCH_NETWORK;
                        
                    case protocol::NodeMessage::BAD_DATA:
                        return NodeError::BAD_DATA;
                        
                    case protocol::NodeMessage::INTERNAL_SERVER_ERROR:
                        return NodeError::INTERNAL_SERVER_ERROR;

                    case protocol::NodeMessage::ALREADY_EXISTS:
                        return NodeError::ALREADY_EXISTS;

                    default:
                        return NodeError::OTHER;
                }
            }


        } /* namespace utils */

    } /* namespace sercli */
} /* namespace sp2p */

