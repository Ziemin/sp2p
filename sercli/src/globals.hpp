#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <boost/asio.hpp>

#include "handler.hpp"

namespace sp2p {
	namespace sercli {

		namespace global {

			extern boost::asio::io_service* io_s;
			extern Sp2pHandler* sp2p_handler;
			extern std::uint64_t node_timeout_seconds;
			extern std::uint32_t max_buffer_size;

			void init(int thread_number);
			void init(int thread_number, boost::asio::io_service* io_s);

			void stop();

			void destroyAll();

		} /* namespace global */

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* GLOBALS_HPP */