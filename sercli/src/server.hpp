#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include <memory>

namespace sp2p {
	namespace sercli {

        /**
		 * Class representing server on some node.
		 */
		class Server : boost::noncopyable, public std::enable_shared_from_this<Server> {

			public:
			private:

		};

		typedef std::shared_ptr<Server> server_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* CLIENT_H */
