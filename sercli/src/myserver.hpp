#ifndef MY_SERVER_H
#define MY_SERVER_H

#include <boost/noncopyable.hpp>
#include <memory>

namespace sp2p {
	namespace sercli {

        /**
		 * Class representing server on some node.
		 */
		class MyServer : boost::noncopyable, public std::enable_shared_from_this<MyServer> {

			public:
				MyServer();
                void start();
                void stop();

			private:

		};

		typedef std::shared_ptr<MyServer> my_server_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MY_SERVER_H */
