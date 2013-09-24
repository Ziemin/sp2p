#ifndef NODEMESSAGE_HPP
#define NODEMESSAGE_HPP

#include "message.hpp"
#include "sp2p_protocol.pb.h"

namespace sp2p {
	namespace sercli {

		/**
		 * Message implementation depicting client message to 
		 * send to Node
		 */
		class NodeRequest : Message {

			public:

				bool SerializeToOstream(std::ostream*);
				bool ParseFromIstream(std::istream*);
				int ByteSize();

				protocol::ClientMessage& getRequest();

			private:

				protocol::ClientMessage request;
				   
		};


		/**
		 * Message implementation depicting node message 
		 * received by client
		 */
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
