#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>

namespace sp2p {
	namespace sercli {

		/**
		 * Abstract class representing a message
		 */
		class Message {
            
			public:

				virtual bool SerializeToOstream(std::ostream*) = 0;

				/**
				 * @returns true if successfully parsed
				 */
				virtual bool ParseFromIstream(std::istream*) = 0;
				virtual int ByteSize() = 0;

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MESSAGE_HPP */
