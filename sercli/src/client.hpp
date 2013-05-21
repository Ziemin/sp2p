#ifndef CLIENT_H
#define CLIENT_H

#include <boost/noncopyable.hpp>
#include <memory>

namespace sp2p {
	namespace sercli {

        /**
		 * Class representing client on some node.
		 */
		class Client : boost::noncopyable, public std::enable_shared_from_this<Client> {

		};

		typedef std::shared_ptr<Client> client_ptr;

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* CLIENT_H */
