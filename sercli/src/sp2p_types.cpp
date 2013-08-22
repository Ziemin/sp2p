#include "sp2p_types.hpp"

namespace sp2p {
    namespace sercli {

        namespace types {


            bool any(const NodeError& error) {
                return error != NodeError::OK;
            }

            bool NetworkDescription::operator<(const NetworkDescription& other) const {
                return this->network_name < other.network_name;

            }

            // NodeDescription
            bool NodeDescription::operator<(const NodeDescription& other) const {
                return (this->ip_address < other.ip_address);
            }
    }
}

