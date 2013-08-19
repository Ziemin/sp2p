#include "network_description.hpp"

namespace sp2p {
	namespace sercli {

		bool NetworkDescription::operator<(const NetworkDescription& other) const {
			return this->network_name < other.network_name;
		}

	}
}
