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
                    << "port: " << node_desc.port;

                return os;
            }
        }
    }
}

