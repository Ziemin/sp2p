#include <thread>
#include <functional>
#include <memory>
#include <limits>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "globals.hpp"
#include "logging.hpp"

namespace blg_lg = boost::log;
namespace blg_src = boost::log::sources;
using namespace blg_lg::trivial;

namespace sp2p {
	namespace sercli {

		namespace global {

			boost::asio::io_service* io_s = nullptr;
			Sp2pHandler* sp2p_handler = nullptr;

			std::uint64_t node_timeout_seconds = 20;  // for a while
			std::uint32_t max_buffer_size = std::numeric_limits<std::uint32_t>::max();


			Sp2pInitializer::Sp2pInitializer(int thread_count, boost::asio::io_service* io_s) 
				: lg(logging::sp2p_lg::get()) 
			{

				logging::init(); // logging intialization
				BOOST_LOG_SEV(lg, trace) << "Initialization of sp2p library";

				if(io_s == nullptr) global::io_s = new boost::asio::io_service();
				else global::io_s = io_s;

				sp2p_handler = new Sp2pHandler();


				for(int i = 0; i < thread_count; i++) {
					std::shared_ptr<std::thread> thr(new std::thread(
								[&]() { global::io_s->run(); }));

					thread_pool.push_back(thr);
				}
			}

			Sp2pInitializer::~Sp2pInitializer() {

				BOOST_LOG_SEV(lg, trace) << "Destruction of sp2p library";

				io_s->stop();
				for(auto tr: thread_pool) tr->join();
				delete sp2p_handler;
				delete io_s;
				//blg_lg::core::get()->remove_all_sinks(); 
			}


		} /* namespace global */

	} /* namespace sercli */
} /* namespace sp2p */

