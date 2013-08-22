#ifndef MESSAGE_FACTORY_HPP
#define MESSAGE_FACTORY_HPP

#include <memory>
#include <botan/botan.h>

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

            std::shared_ptr<NodeRequest> getRegisterUserMessage(const MyUser& my_user, const std::string& public_key);

            std::shared_ptr<NodeRequest> getUnregisterUserMessage(const std::string& cookie);

            std::shared_ptr<NodeRequest> getChangePasswordMessage(const MyUser& my_user, 
                    const std::string& new_password, const std::string& cookie);

            std::shared_ptr<NodeRequest> getUserInfoMessage(const NetworkDescription& network_desc, 
                        const std::string& username, const std::string& cookie);

            std::shared_ptr<NodeRequest> getListNetworksMessage(const std::string& cookie);

            std::shared_ptr<NodeRequest> getListMyNetworksMessage(const std::string& cookie);

            std::shared_ptr<NodeRequest> getListServersMessage(const NetworkDescription& network_desc, const std::string& cookie);

            std::shared_ptr<NodeRequest> getCreateNetworkMessage(const NetworkDescription& network_desc, const std::string& cookie);

            std::shared_ptr<NodeRequest> getDeleteNetworkMessage(const NetworkDescription& network_desc, const std::string& cookie);

            std::shared_ptr<NodeRequest> getInviteUserMessage(const NetworkDescription& network_desc, const User& user,
                    const std::string& cookie);

            std::shared_ptr<NodeRequest> getRemoveUserMessage(const NetworkDescription& network_desc, const User& user,
                    const std::string& cookie);

            std::shared_ptr<NodeRequest> getUpdateServerMessage(const NetworkDescription& network_desc, 
                    const std::int32_t port, const std::string& cookie);

            std::shared_ptr<NodeRequest> getStopServerMessage(const NetworkDescription& network_desc, const std::string& cookie);

            std::shared_ptr<NodeRequest> getSignKeyMessage(const Botan::Public_Key& public_key,
                   const NetworkDescription* network_desc, const std::string& cookie);


            NetworkDescription getNetworkDescriptionFromProto(const protocol::NodeMessage::ListNetworks::Network& network);

            ServerDescription getServerDescriptionFromProto(const protocol::NodeMessage::ListServers::Server& server,
                    const NetworkDescription& network_desc);

            NodeError getDefaultError(protocol::NodeMessage::ResponseType responseCode);

        } /* namespace utils */

    } /* namespace sercli */
} /* namespace sp2p */

#endif /* MESSAGE_FACTORY_HPP */
