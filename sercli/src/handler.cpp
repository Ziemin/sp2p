
#include "handler.hpp"

 namespace sp2p {
	namespace sercli {
 

		void Sp2pHandler::handleResponse(Connection<NodeRequest, NodeResponse>& connection, 
				NodeResponse& response) 
		{
			// TODO
		}

		void handleSendError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			connection.stop();
		}

		void handleReadError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			connection.stop();
		}

		void handleParseError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			connection.stop();
		}

	} /* namespace sercli */
 } /* namespace sp2p */
