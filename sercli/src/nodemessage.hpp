#ifndef NODEMESSAGE_HPP
#define NODEMESSAGE_HPP

#include "message.hpp"
#include "sp2p_protocol.pb.h"

namespace sp2p {
	namespace sercli {

		class NodeRequest : Message {

			public:

				bool SerializeToOstream(std::ostream*);
				bool ParseFromIstream(std::istream*);
				int ByteSize();

				protocol::ClientMessage& getRequest();

			private:

				protocol::ClientMessage request;
				   
		};


		class NodeResponse : Message {

			public:

				bool SerializeToOstream(std::ostream*);
				bool ParseFromIstream(std::istream*);
				int ByteSize();

				protocol::NodeMessage& getResponse();

			private:

				protocol::NodeMessage response;
				   
		};


	} /* namespace sercli */
} /* namespace sp2p */

#endif /* CONNECTION_HPP */
