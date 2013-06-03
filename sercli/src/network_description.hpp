#ifndef NETWORK_DESCRIPTION_HPP
#define NETWORK_DESCRIPTION_HPP

#include <string>
#include "user.hpp"

namespace sp2p {
	namespace sercli {

		namespace network {

			enum class AccessRights { PUBLIC, PRIVATE };
			enum class Visibility { LISTED, UNLISTED };
			enum class ParticipationRights { CLIENT_ONLY, CLIENT_SERVER };

		} /* namespace network_utilities */

		struct NetworkDescription {
			network::AccessRights access_rights;
			network::Visibility  visability;
			network::ParticipationRights participation_rights;
			std::string protocol_name;
			std::string network_name;

			User creator;

			bool operator<(const NetworkDescription& other) const;
		};

	} /* namespace sercli */
} /* namespace sp2p */
#endif /* NETWORK_DESCRIPTION_HPP */
