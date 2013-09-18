#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <boost/asio.hpp>
#include <botan/botan.h>

//#include "handler.hpp"
#include "logging.hpp"


namespace sp2p {
	namespace sercli {

        class NodeRequest;
        class NodeResponse;

        template <typename Request, typename Response> class Handler;
        template <typename Request, typename Response>
            using handler_ptr = std::shared_ptr<Handler<Request, Response>>;

		namespace global {


			extern boost::asio::io_service* io_s;
			extern handler_ptr<NodeRequest, NodeResponse> sp2p_handler;
			extern std::uint64_t node_timeout_seconds;
			extern std::uint32_t max_buffer_size;

			class Sp2pInitializer {

				public:
					Sp2pInitializer(int thread_number, boost::asio::io_service* io_s=nullptr);
					~Sp2pInitializer();

				private:
					boost::asio::io_service::work* work;
					std::vector<std::shared_ptr<std::thread>> thread_pool;
					logging::Logger& lg;
					Botan::LibraryInitializer init;
			};

		} /* namespace global */
	} /* namespace sercli */
} /* namespace sp2p */

#endif /* GLOBALS_HPP */
