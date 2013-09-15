#include "sp2p_types.hpp"

namespace sp2p {
    namespace sercli {

        namespace types {

            std::ostream& operator<<(std::ostream& os, const ServerDescription& server_desc) {
                os << "Server -> ip: " << server_desc.ip_addres.to_string() 
                    << ", user: " << server_desc.related_user << ", network: " << server_desc.related_network;
                return os;
            }

			std::ostream& operator<<(std::ostream& os, const NetworkDescription& net_desc) {
                os << "Network - > name: " << net_desc.network_name 
                    << ", protocol: " << net_desc.protocol_name 
                    << "creator: " << net_desc.creator;

                os << " ,access rights: ";
                switch(net_desc.access_rights) {
                    case NetworkDescription::AccessRights::PRIVATE:
                        os << "private";
                        break;
                    case NetworkDescription::AccessRights::PUBLIC:
                        os << "public";
                        break;
                }

                os << ", participation rights: ";

                switch(net_desc.participation_rights) {
                    case NetworkDescription::ParticipationRights::CLIENT_ONLY:
                        os << "client only";
                        break;
                    case NetworkDescription::ParticipationRights::CLIENT_SERVER:
                        os << "client/server";
                        break;
                }

                os << ", visibility: ";
                switch(net_desc.visability) {
                    case NetworkDescription::Visibility::LISTED:
                        os << "listed";
                        break;
                    case NetworkDescription::Visibility::UNLISTED:
                        os << "unlisted";
                        break;
                } 
                return os;
            }

			std::ostream& operator<<(std::ostream& os, const NodeDescription& node_desc) {
                os << "Node - > name: " << node_desc.node_name
                    << ", ip: " << node_desc.ip_address.to_string() 
                    << ", port: " << node_desc.port;

                return os;
            }

			std::ostream& operator<<(std::ostream& os, const NodeError& error) {
                switch(error) {
                    case NodeError::OK:
                        os << "OK";
                        break;
                    case NodeError::IS_ACTIVE:
                        os << "IS_ACTIVE";
                        break;
                    case NodeError::NO_CONNECTION:
                        os << "NO_CONNECTION";
                        break;
                    case NodeError::INTERNAL_SERVER_ERROR:
                        os << "INTERNAL_SERVER_ERROR";
                        break;
                    case NodeError::BAD_REQUEST:
                        os << "BAD_REQUEST";
                        break;
                    case NodeError::NO_PRIVILAGES:
                        os << "NO_PRIVILAGES";
                        break;
                    case NodeError::BAD_CREDENTIALS:
                        os << "BAD_CREDENTIALS";
                        break;
                    case NodeError::NOT_LOGGED:
                        os << "NOT_LOGGED";
                        break;
                    case NodeError::NO_SUCH_USER:
                        os << "NO_SUCH_USER";
                        break;
                    case NodeError::NO_SUCH_NETWORK:
                        os << "NO_SUCH_NETWORK";
                        break;
                    case NodeError::BAD_DATA:
                        os << "BAD_DATA";
                        break;
                    case NodeError::ALREADY_EXISTS:
                        os << "ALREADY_EXISTS";
                        break;
                    case NodeError::BAD_SERVER_RESPONSE:
                        os << "BAD_SERVER_RESPONSE";
                        break;
                    case NodeError::SEND_ERROR:
                        os << "SEND_ERROR";
                        break;
                    case NodeError::OTHER:
                        os << "OTHER";
                        break;
                }
                return os;
            }

            size_t NetworkHash::operator()(const NetworkDescription& net_desc) const {
                return hash_fn(net_desc.network_name);
            }
        }
    }
}

