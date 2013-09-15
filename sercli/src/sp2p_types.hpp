#ifndef SP2P_TYPES_HPP
#define SP2P_TYPES_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>

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
				ALREADY_EXISTS,

				BAD_SERVER_RESPONSE,
				SEND_ERROR,
				OTHER
			};

			std::ostream& operator<<(std::ostream& os, const NodeError& error);

			inline bool any(const NodeError& error) {
                return error != NodeError::OK;
			}


			struct ServerDescription {

				std::string related_user;
				std::string related_network;
				boost::asio::ip::address ip_addres;

				// serialization
				private: 

					friend std::ostream& operator<<(std::ostream& os, const ServerDescription& server_desc);

					friend class boost::serialization::access;

					template<class Archive> void save(Archive& ar, const unsigned int /* version */) const {
						ar & related_user;
						ar & related_network;
						std::string ip_str = ip_addres.to_string();
						ar & ip_str;
					}

					template<class Archive> void load(Archive& ar, const unsigned int /* version */) {
						ar & related_user;
						ar & related_network;
						std::string ip_str;
						ar & ip_str;
						ip_addres = boost::asio::ip::address::from_string(ip_str);
					}
					BOOST_SERIALIZATION_SPLIT_MEMBER();

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

				inline bool operator==(const NetworkDescription& other) const {
					return this->network_name == other.network_name;

				}

				// serialization
				private: 
					friend std::ostream& operator<<(std::ostream& os, const NetworkDescription& net_desc);

					friend class boost::serialization::access;
					template<class Archive> void serialize(Archive& ar, const unsigned int /* version */) {
						ar & access_rights & visability & participation_rights;
						ar & protocol_name & network_name;
						ar & creator;
					}
			};

            struct NetworkHash {

                size_t operator()(const NetworkDescription& net_desc) const;

                private:
                    std::hash<std::string> hash_fn;
            };

			struct NodeDescription {
				boost::asio::ip::address ip_address;
				std::string node_name;
				std::uint16_t port;

				inline bool operator<(const NodeDescription& other) const {
					return this->node_name < other.node_name;
				}

				// serialization
				private: 
					friend std::ostream& operator<<(std::ostream& os, const NodeDescription& node_desc);

					friend class boost::serialization::access;

					template<class Archive> void save(Archive& ar, const unsigned int /* version */) const {
						std::string ip_str = ip_address.to_string();
						ar & ip_str;
						ar & node_name;
						ar & port;
					}

					template<class Archive> void load(Archive& ar, const unsigned int /* version */) {
						std::string ip_str;
						ar & ip_str;
						ar & node_name;
						ar & port;

						ip_address = boost::asio::ip::address::from_string(ip_str);
					}
					BOOST_SERIALIZATION_SPLIT_MEMBER();
			};

		}
		// Error from node and nodeclass
	}
}

#endif /* SP2P_TYPES_HPP */
