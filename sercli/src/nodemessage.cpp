#include "nodemessage.hpp"

namespace sp2p {
	namespace sercli {

		// NodeRequest implementation  -----------------------------------

		bool NodeRequest::SerializeToOstream(std::ostream* os) {
			return request.SerializeToOstream(os);
		}

		bool NodeRequest::ParseFromIstream(std::istream* is) {
			return request.ParseFromIstream(is);
		}

		protocol::ClientMessage& NodeRequest::getRequest() {
			return request;
		}

		int NodeRequest::ByteSize() {
			return request.ByteSize();
		}


		// NodeResponse implementation  -----------------------------------

		bool NodeResponse::SerializeToOstream(std::ostream* os) {
			return response.SerializeToOstream(os);
		}

		bool NodeResponse::ParseFromIstream(std::istream* is) {
			return response.ParseFromIstream(is);
		}

		protocol::NodeMessage& NodeResponse::getResponse() {
			return response;
		}

		int NodeResponse::ByteSize() {
			return response.ByteSize();
		}

	} /* namespace sercli */
} /* namespace sp2p */

