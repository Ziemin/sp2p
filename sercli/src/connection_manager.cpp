#include "connection.hpp"

namespace sp2p {
	namespace sercli {

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::start(connection_ptr<Request, Response> con) {

				connections.insert(con);
				con->go();
			}

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::stop(connection_ptr<Request, Response> con) {

				con->stop();
				connections.erase(con);
			}

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::gracefulStop(connection_ptr<Request, Response> con) {
				con->gracefulStop();
				connections.erase(con);
			}

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::stopAll() {

				for(auto con: connections) con->stop();
				connections.clear();
			}

	} /* namespace sercli */
} /* namespace sp2p */
