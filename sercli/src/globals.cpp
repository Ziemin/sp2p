#include <thread>
#include <functional>
#include <memory>
#include <limits>

#include "globals.hpp"

namespace sp2p {
	namespace sercli {

		namespace global {

			boost::asio::io_service* io_s = nullptr;
			Sp2pHandler* sp2p_handler = nullptr;

			std::uint64_t node_timeout_seconds = 20;  // for a while
			std::uint32_t max_buffer_size = std::numeric_limits<std::uint32_t>::max();


			struct LibraryInitializer {

				std::vector<std::shared_ptr<std::thread>> thread_pool;

				LibraryInitializer(int thread_count, boost::asio::io_service* io_s) {

					if(io_s == nullptr) global::io_s = new boost::asio::io_service();
					else global::io_s = io_s;

					sp2p_handler = new Sp2pHandler();

					for(int i = 0; i < thread_count; i++) {
						std::shared_ptr<std::thread> thr(new std::thread(
									[&]() { global::io_s->run(); }));

						thread_pool.push_back(thr);
					}
				}

				~LibraryInitializer() {
					io_s->stop();
					for(auto tr: thread_pool) tr->join();
					delete sp2p_handler;
					delete io_s;
				}
			};

			std::shared_ptr<LibraryInitializer> initializer = nullptr;

			void init(int thread_number, boost::asio::io_service* io_s) {
				if(initializer == nullptr) initializer.reset(new LibraryInitializer(thread_number, io_s));
			}

			void destroyAll() {
				initializer.reset();
			}

		} /* namespace global */

	} /* namespace sercli */
} /* namespace sp2p */

