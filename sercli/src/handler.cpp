
#include "handler.hpp"
#include <boost/log/core.hpp>

 namespace sp2p {
	namespace sercli {
 
		using namespace boost::log::trivial;

		void Sp2pHandler::handleResponse(Connection<NodeRequest, NodeResponse>& connection, 
				NodeResponse& response) {
			// TODO
			BOOST_LOG_SEV(lg, trace) << "handling resposne";
		}

		void Sp2pHandler::handleSendError(Connection<NodeRequest, NodeResponse>& connection, NodeRequest& request) {
			// TODO - coś ciekawszego
			BOOST_LOG_SEV(lg, trace) << "handling sending error";
			connection.stop();
		}

		void Sp2pHandler::handleReadError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			BOOST_LOG_SEV(lg, trace) << "handling reading error";
			connection.stop();
		}

		void Sp2pHandler::handleParseError(Connection<NodeRequest, NodeResponse>& connection) {
			// TODO - coś ciekawszego
			BOOST_LOG_SEV(lg, trace) << "handling parsing error";
			connection.stop();
		}

	} /* namespace sercli */
 } /* namespace sp2p */
