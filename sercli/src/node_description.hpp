#ifndef NODE_DESCRIPTION_HPP
#define NODE_DESCRIPTION_HPP

#include <string>
#include <boost/asio.hpp>
#include "user.hpp"

namespace sp2p {
	namespace sercli {

		struct NodeDescription {
			boost::asio::ip::address ip_address;
			std::string node_name;
			std::uint32_t port;

			bool operator<(const NodeDescription& other) const;
		};

	} /* namespace sercli */
} /* namespace sp2p */
#endif /* NODE_DESCRIPTION_HPP */
