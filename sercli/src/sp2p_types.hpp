#ifndef SP2P_TYPES_HPP
#define SP2P_TYPES_HPP

#include <string>
#include <boost/asio.hpp>

#include "user.hpp"

namespace sp2p {
	namespace sercli {

		namespace types {


			enum class NodeError {
				OK = 0, 		// to user like bool value

				IS_ACTIVE,
				NO_CONNECTION,

				INTERNAL_SERVER_ERROR,
				BAD_REQUEST,
				NO_PRIVILAGES,
				BAD_CREDENTIALS,
				NOT_LOGGED,
				NO_SUCH_USER,
				NO_SUCH_NETWORK,
				BAD_DATA,

				BAD_SERVER_RESPONSE,
				SEND_ERROR,
				OTHER
			};

			inline bool any(const NodeError& error) {
                return error != NodeError::OK;
			}


			struct ServerDescription {

				std::string related_user;
				std::string related_network;
				boost::asio::ip::address ip_addres;

			};

			struct NetworkDescription {

				enum class AccessRights { PUBLIC, PRIVATE };
				enum class Visibility { LISTED, UNLISTED };
				enum class ParticipationRights { CLIENT_ONLY, CLIENT_SERVER };

				AccessRights access_rights;
				Visibility  visability;
				ParticipationRights participation_rights;
				std::string protocol_name;
				std::string network_name;

				User creator;

				inline bool operator<(const NetworkDescription& other) const {
					return this->network_name < other.network_name;

				}
			};


			struct NodeDescription {
				boost::asio::ip::address ip_address;
				std::string node_name;
				std::uint32_t port;

				inline bool operator<(const NodeDescription& other) const {
					return (this->ip_address < other.ip_address);
				}
			};

		}
		// Error from node and nodeclass
	}
}

#endif /* SP2P_TYPES_HPP */
