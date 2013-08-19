#include "node_description.hpp"

namespace sp2p {
	namespace sercli {

		// NodeDescription
		bool NodeDescription::operator<(const NodeDescription& other) const {
			return (this->ip_address < other.ip_address);
		}

	}
}
