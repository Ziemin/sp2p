#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <memory>
#include <boost/noncopyable.hpp>

namespace sp2p {
	namespace sercli {

		/**
		 * Connection spawned by server
		 */
		class Connection : boost::noncopyable, 
		public std::enable_shared_from_this<Connection> { 

			public:
				Connection();

				void start();
				void handleRead();
				void handleWrite();

			private:

		};

		typedef std::shared_ptr<Connection> connection_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* TO_SERVER_CONNECTION_HPP */
