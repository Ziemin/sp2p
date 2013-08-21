#include "node.hpp"
#include "message_factory.hpp"

using namespace sp2p::sercli::types;

namespace sp2p {
	namespace sercli {


		Node::Node(NodeDescription node_desc, 
				ConnectionManager<NodeRequest, NodeResponse>& connection_manager) 
			: node_desc(node_desc),
			my_user("user", "password", "email"),
			node_connection(connection_manager)
		{
		}

		// Node
		Node::Node(NodeDescription node_desc, 
				ConnectionManager<NodeRequest, NodeResponse>& connection_manager, 
				MyUser user)
			: node_desc(node_desc), 
			my_user(user), 
			node_connection(connection_manager)
		{ 
		}

		bool Node::isActive() const {
			return node_connection.isActive();
		}

		bool Node::isRegistered() const {
			return this->my_user.is_registered;
		}

		void Node::setNewDescription(NodeDescription node_desc) {
			if(this->isActive()) 
				throw NodeException("Node is active at the moment");

			this->node_desc = node_desc;
		}

		NodeError Node::beforeMessage() {
			if(!node_connection.is_logged) 
				return NodeError::NOT_LOGGED;

			try {
				if(!isActive()) node_connection.connect(node_desc);
				else node_connection.resetDeadlineTimer(global::node_timeout_seconds);

				return NodeError::OK;
			} catch(NodeError& error) {
				return error;
			}
		}

		NodeError Node::logIn() {

			NodeError result = NodeError::OK;

			if(!node_connection.is_logged) {
				try {
					if(!isActive()) node_connection.connect(node_desc);
					else node_connection.resetDeadlineTimer(global::node_timeout_seconds);
				} catch(NodeError& error) {
					return error;
				}

				std::shared_ptr<NodeRequest> loginMessage = 
					utils::getLoginMessage(my_user.username, my_user.password);

				Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
				try {
					NodeResponse nodeResponse = con->sendRequest(loginMessage).get();
					protocol::NodeMessage& response = nodeResponse.getResponse();

					switch(response.response_type()) {

						case protocol::NodeMessage::OK:
							if(response.has_login_response()) {
								const protocol::NodeMessage::Login& loginResponse = response.login_response();
								if(loginResponse.has_cookie()) {
									result = NodeError::OK;
									node_connection.setCookie(loginResponse.cookie());
									node_connection.is_logged = true;
									break;
								}
							} 
							result = NodeError::BAD_SERVER_RESPONSE;
							break;

						default:
							result = utils::getDefaultError(response.response_type());
					}

					node_connection.resetDeadlineTimer(global::node_timeout_seconds);

				} catch(SendException& e) {
					return NodeError::SEND_ERROR;
				}
			}

			return result;
		}


		NodeError Node::logOut() {

			NodeError result = beforeMessage();
			if(any(result)) return result;

			std::shared_ptr<NodeRequest> logoutMessage = utils::getLogoutMessage(node_connection.getCookie());

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(logoutMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						node_connection.is_logged = false;
						node_connection.setCookie("");
						result = NodeError::OK;
						break;

					default:
						result = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				return NodeError::SEND_ERROR;
			}

			return result;
		}

		NodeError Node::registerUser() {

			if(my_user.is_registered) return NodeError::OK;

			NodeError result = beforeMessage();
			if(any(result)) return result;

			std::shared_ptr<NodeRequest> registerUserMessage = utils::getRegisterUserMessage(my_user, ""); // TODO implement

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(registerUserMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						my_user.is_registered = true;
						// TODO certificates
						result = NodeError::OK;
						break;

					default:
						result = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				return NodeError::SEND_ERROR;
			}

			return result;
		}

		NodeError Node::changePassword(const std::string& new_password) {

			NodeError result = beforeMessage();
			if(any(result)) return result;

			std::shared_ptr<NodeRequest> changePasswordMessage = 
				utils::getChangePasswordMessage(my_user, new_password, node_connection.getCookie());

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(changePasswordMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						my_user.password = new_password;
						result = NodeError::OK;
						break;

					default:
						result = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				return NodeError::SEND_ERROR;
			}

			return result;
		}


		std::tuple<NodeError, User> Node::getUserInfo(const NetworkDescription& network_desc, const std::string& username) {

			std::tuple<NodeError, User> resultTuple;

			NodeError error = beforeMessage();
			if(any(error)) {
				std::get<0>(resultTuple) = error;
				return resultTuple;
			}

			std::shared_ptr<NodeRequest> userInfoMessage = 
				utils::getUserInfoMessage(network_desc, username, node_connection.getCookie());

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(userInfoMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						std::get<1>(resultTuple) = { username, "" }; //TODO public key
						error = NodeError::OK;
						break;

					default:
						error = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				error = NodeError::SEND_ERROR;
			}
			std::get<0>(resultTuple) = error;
			return resultTuple;
		}
		
		std::tuple<NodeError, std::vector<NetworkDescription>> Node::getNetworksList() {

			std::tuple<NodeError, std::vector<NetworkDescription>> resultTuple;

			NodeError error = beforeMessage();
			if(any(error)) {
				std::get<0>(resultTuple) = error;
				return resultTuple;
			}

			std::shared_ptr<NodeRequest> listNetworksMessage = 
				utils::getListNetworksMessage(node_connection.getCookie());

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(listNetworksMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						{
							std::vector<NetworkDescription>& networks = std::get<1>(resultTuple);

							const protocol::NodeMessage::ListNetworks& netList = response.list_networks_response();
							for(int i = 0; i < netList.network_list_size(); i++) {
								const protocol::NodeMessage::ListNetworks::Network& nodeNetwork = netList.network_list(i);
								networks.push_back(utils::getNetworkDescriptionFromProto(nodeNetwork));
							}

							error = NodeError::OK;
							break;
						}

					default:
						error = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				error = NodeError::SEND_ERROR;
			}

			std::get<0>(resultTuple) = error;
			return resultTuple;
		}

		std::tuple<NodeError, std::vector<NetworkDescription>> Node::getMyNetworks() {

			std::tuple<NodeError, std::vector<NetworkDescription>> resultTuple;

			NodeError error = beforeMessage();
			if(any(error)) {
				std::get<0>(resultTuple) = error;
				return resultTuple;
			}

			std::shared_ptr<NodeRequest> listMyNetworksMessage = 
				utils::getListMyNetworksMessage(node_connection.getCookie());

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(listMyNetworksMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						{
							std::vector<NetworkDescription>& networks = std::get<1>(resultTuple);

							const protocol::NodeMessage::ListMyNetworks& netList = response.list_my_networks_response();
							for(int i = 0; i < netList.network_list_size(); i++) {
								const protocol::NodeMessage::ListNetworks::Network& nodeNetwork = netList.network_list(i);
								networks.push_back(utils::getNetworkDescriptionFromProto(nodeNetwork));
							}

							error = NodeError::OK;
							break;
						}

					default:
						error = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				error = NodeError::SEND_ERROR;
			}

			std::get<0>(resultTuple) = error;
			return resultTuple;
		}

		std::tuple<NodeError, std::vector<ServerDescription>> Node::getServersList(const NetworkDescription& network_desc) {

			std::tuple<NodeError, std::vector<ServerDescription>> resultTuple;

			NodeError error = beforeMessage();
			if(any(error)) {
				std::get<0>(resultTuple) = error;
				return resultTuple;
			}

			std::shared_ptr<NodeRequest> listServersMessage = 
				utils::getListServersMessage(network_desc, node_connection.getCookie());

			Connection<NodeRequest, NodeResponse>* con = node_connection.getConnection().get();
			try {
				NodeResponse nodeResponse = con->sendRequest(listServersMessage).get();
				protocol::NodeMessage response = nodeResponse.getResponse();

				switch(response.response_type()) {
					case protocol::NodeMessage::OK:
						{
							std::vector<ServerDescription>& servers = std::get<1>(resultTuple);

							const protocol::NodeMessage::ListServers& serverList = response.list_servers_response();
							for(int i = 0; i < serverList.list_servers_size(); i++) {
								const protocol::NodeMessage::ListServers::Server& nodeServer = serverList.list_servers(i);
								servers.push_back(utils::getServerDescriptionFromProto(nodeServer));
							}

							error = NodeError::OK;
							break;
						}

					default:
						error = utils::getDefaultError(response.response_type());
				}

				node_connection.resetDeadlineTimer(global::node_timeout_seconds);

			} catch(SendException& e) {
				error = NodeError::SEND_ERROR;
			}

			std::get<0>(resultTuple) = error;
			return resultTuple;
		}

	} /* namespace sercli */

} /* namespace sp2p */
