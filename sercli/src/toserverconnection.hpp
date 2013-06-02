#ifndef TO_SERVER_CONNECTION_HPP
#define TO_SERVER_CONNECTION_HPP

#include <memory>
#include <boost/noncopyable.hpp>

namespace sp2p {
	namespace sercli {

		/**
		 * Connection with servers
		 */
		class ToServerConnection : boost::noncopyable, 
		public std::enable_shared_from_this<ToServerConnection> { 

			public:
				ToServerConnection();

				template <typename ConnectHandler>
					void start(ConnectHandler handler);

				template <typename WriteHandler>
					void write(WriteHandler handler);

				template <typename ReadHandler>
					void handleRead(ReadHandler handler);

				template <typename StopHandler>
					void stop(StopHandler handler);

			private:

		};

		typedef std::shared_ptr<ToServerConnection> tsc_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* TO_SERVER_CONNECTION_HPP */
