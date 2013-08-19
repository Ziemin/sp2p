#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>

namespace sp2p {
	namespace sercli {

		class Message {
            
			public:

				virtual bool SerializeToOstream(std::ostream*) = 0;
				virtual bool ParseFromIstream(std::istream*) = 0;
				virtual int ByteSize() = 0;

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MESSAGE_HPP */
