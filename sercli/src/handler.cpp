
#include "handler.hpp"

 namespace sp2p {
	namespace sercli {
 

		void Sp2pHandler::handleResponse(Connection<NodeRequest, NodeResponse>& connection, 
				NodeResponse& response) {
			// TODO
		}

		void Sp2pHandler::handleSendError(Connection<NodeRequest, NodeResponse>& connection, NodeRequest& request) {
			// TODO - coś ciekawszego
			connection.stop();
		}

		void Sp2pHandler::handleReadError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			connection.stop();
		}

		void Sp2pHandler::handleParseError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			connection.stop();
		}

	} /* namespace sercli */
 } /* namespace sp2p */
