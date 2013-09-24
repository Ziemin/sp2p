#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <boost/asio.hpp>
#include <botan/botan.h>
#include <thread>

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


			extern boost::asio::io_service* io_s; 	//!< Global instance of boost's io_service

			extern handler_ptr<NodeRequest, NodeResponse> sp2p_handler; //!< Global instance of handler for connections
																		//!< related to communication with Node
			extern std::uint64_t node_timeout_seconds; 	//!< time limit for all io operations in communication with Node
			extern std::uint32_t max_buffer_size; 	//!< maximum size of buffer for messages delivered by Node

			/** Library initializer class
			 *
			 *  This class is intended to initialize all required data for Sp2p library.
			 *  It is meant to be be alive in the entire scope of library usage.
			 */
			class Sp2pInitializer {

				public:
					/*! Sp2p Library Initializer Constructor
					 * \param thread_number threads living in pool of io_service
					 */
					Sp2pInitializer(int thread_number, boost::asio::io_service* io_s=nullptr);

					/*! Desctructor releasing all memory aquired by library and shutting down entire thread pool*/
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
