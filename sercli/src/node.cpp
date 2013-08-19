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

		NodeError Node::logIn() {

			NodeError result = NodeError::OK;

			if(!node_connection.is_logged) {
				if(!isActive()) node_connection.connect(node_desc);
				else node_connection.resetDeadlineTimer(global::node_timeout_seconds);

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

			if(!node_connection.is_logged) 
				return NodeError::NOT_LOGGED;

			try {
				if(!isActive()) node_connection.connect(node_desc);
			} catch(NodeError& error) {
				return error;
			}

			NodeError result;
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
			return NodeError::OK;
		}

	}

}
