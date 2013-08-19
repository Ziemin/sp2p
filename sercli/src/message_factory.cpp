#include "message_factory.hpp"


using namespace sp2p::sercli::types;

namespace sp2p {
	namespace sercli {

		namespace utils {

			std::shared_ptr<NodeRequest> getLoginMessage(
					const std::string& username, 
					const std::string& password) 
			{

				std::shared_ptr<NodeRequest> request(new NodeRequest());
				protocol::ClientMessage& protoRequest = request->getRequest();
				protoRequest.set_request(protocol::ClientMessage_RequestType_LOGIN);

				protocol::ClientMessage::Login *login_message = new protocol::ClientMessage::Login();
				login_message->set_allocated_username(new std::string(username));
				login_message->set_allocated_password(new std::string(password));

				protoRequest.set_allocated_login_message(login_message);

				return request;
			}



			NodeError getDefaultError(protocol::NodeMessage::ResponseType responseCode) {

				NodeError result;
				switch(responseCode) {

					case protocol::NodeMessage::OK:
						result = NodeError::OK;
						return result;

					case protocol::NodeMessage::BAD_CREDENTIALS:
						result = NodeError::BAD_CREDENTIALS;
						return result;
						
					case protocol::NodeMessage::BAD_REQUEST:
						result = NodeError::BAD_REQUEST;
						return result;
						
					case protocol::NodeMessage::NO_PRIVILAGES:
						result = NodeError::NO_PRIVILAGES;
						return result;
						
					case protocol::NodeMessage::NO_SUCH_USER:
						result = NodeError::NO_SUCH_USER;
						return result;
						
					case protocol::NodeMessage::NO_SUCH_NETWORK:
						result = NodeError::NO_SUCH_NETWORK;
						return result;
						
					case protocol::NodeMessage::BAD_DATA:
						result = NodeError::BAD_DATA;
						return result;
						
					case protocol::NodeMessage::INTERNAL_SERVER_ERROR:
						result = NodeError::INTERNAL_SERVER_ERROR;
						return result;

					default:
						result = NodeError::OTHER;
						return result;
				}
			}


		} /* namespace utils */

	} /* namespace sercli */
} /* namespace sp2p */

