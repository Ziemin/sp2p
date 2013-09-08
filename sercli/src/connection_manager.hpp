#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <set>

#include "connection.hpp"
#include "logging.hpp"

namespace sp2p {
	namespace sercli {

		template <typename Request, typename Response>
			class Connection;

		template <typename Request, typename Response>
			using connection_ptr = std::shared_ptr<Connection<Request, Response>>;

		template <typename Request, typename Response>
			class ConnectionManager {

				public:

					void start(connection_ptr<Request, Response> con);
					void stop(connection_ptr<Request, Response> con);
					void gracefulStop(connection_ptr<Request, Response> con);
					void stopAll();

				private:

					std::set<connection_ptr<Request, Response>> connections;
					logging::Logger& lg = logging::sp2p_lg::get();

			};

	} /* namespace sercli */
} /* namespace sp2p */

#include "connection_manager.cpp"

#endif /* CONNECTION_MANAGER */
