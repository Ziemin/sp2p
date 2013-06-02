#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include <memory>
#include "toserverconnection.hpp"

namespace sp2p {
	namespace sercli {

        /**
		 * Class representing server on some node.
		 */
		class Server {

			public:
				Server();
        		tsc_ptr getConnection();

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* SERVER_H */
