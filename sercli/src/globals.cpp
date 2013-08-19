#include <thread>
#include <functional>
#include <memory>

#include "globals.hpp"

namespace sp2p {
	namespace sercli {

		namespace global {

			boost::asio::io_service* io_s = nullptr;
			Sp2pHandler* sp2p_handler = nullptr;
			std::uint64_t node_timeout_seconds = 20;  // for a while

			std::vector<std::shared_ptr<std::thread>> thread_pool;

			void init(int thread_number, boost::asio::io_service* io_s=nullptr) {

				if(io_s == nullptr) io_s = new boost::asio::io_service();
				else sp2p::sercli::global::io_s = io_s;

				sp2p_handler = new Sp2pHandler();

				for(int i = 0; i < thread_number; i++) {
					std::shared_ptr<std::thread> thr(new std::thread(
								[=]() { io_s->run(); }));

					thread_pool.push_back(thr);
				}
			}

			void stop() {
				io_s->stop();
				for(auto t: thread_pool) t->join();
				io_s->reset();
			}

			void destroyAll() {
				delete io_s;
				delete sp2p_handler;
			}

		} /* namespace global */

	} /* namespace sercli */
} /* namespace sp2p */
