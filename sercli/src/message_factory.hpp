#ifndef MESSAGE_FACTORY_HPP
#define MESSAGE_FACTORY_HPP

#include <memory>

#include "sp2p_protocol.pb.h"
#include "nodemessage.hpp"
#include "sp2p_types.hpp"
#include "user.hpp"

using namespace sp2p::sercli::types;

namespace sp2p {
	namespace sercli {

		namespace utils {

			std::shared_ptr<NodeRequest> getLoginMessage(const std::string& username, const std::string& password);
			std::shared_ptr<NodeRequest> getLogoutMessage(const std::string& cookie);
			std::shared_ptr<NodeRequest> getRegisterUserMessage(const MyUser& my_user);

			NodeError getDefaultError(protocol::NodeMessage::ResponseType responseCode);

		} /* namespace utils */

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MESSAGE_FACTORY_HPP */
