#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <boost/asio.hpp>

#include "handler.hpp"
#include "logging.hpp"

namespace sp2p {
	namespace sercli {

		namespace global {

			extern boost::asio::io_service* io_s;
			extern Sp2pHandler* sp2p_handler;
			extern std::uint64_t node_timeout_seconds;
			extern std::uint32_t max_buffer_size;

			class Sp2pInitializer {

				public:
					Sp2pInitializer(int thread_number, boost::asio::io_service* io_s=nullptr);
					~Sp2pInitializer();

				private:
					std::vector<std::shared_ptr<std::thread>> thread_pool;
					logging::Logger& lg;
			};

		} /* namespace global */

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* GLOBALS_HPP */
