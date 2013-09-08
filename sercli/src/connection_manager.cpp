#include "connection.hpp"
#include <boost/log/core.hpp>

namespace sp2p {
	namespace sercli {

		using namespace boost::log::trivial;

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::start(connection_ptr<Request, Response> con) {

                BOOST_LOG_SEV(lg, trace) << "ConnectionManager: Starting connection";
				connections.insert(con);
				con->go();
			}

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::stop(connection_ptr<Request, Response> con) {

                BOOST_LOG_SEV(lg, trace) << "ConnectionManager: Stopping connection";
				con->stop();
				connections.erase(con);
			}

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::gracefulStop(connection_ptr<Request, Response> con) {
                BOOST_LOG_SEV(lg, trace) << "ConnectionManager: Gracefully stopping connection connection";
				con->gracefulStop();
				connections.erase(con);
			}

		template<typename Request, typename Response>
			void ConnectionManager<Request, Response>::stopAll() {
                BOOST_LOG_SEV(lg, trace) << "ConnectionManager: Stopping all connections";
				for(auto con: connections) con->stop();
				connections.clear();
			}

	} /* namespace sercli */
} /* namespace sp2p */
