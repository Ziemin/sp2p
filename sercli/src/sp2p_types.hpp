#ifndef SP2P_TYPES_HPP
#define SP2P_TYPES_HPP

namespace sp2p {
	namespace sercli {

		namespace types {


			enum class NodeError {
				OK = 0, 		// to user like bool value

				IS_ACTIVE,
				NO_CONNECTION,

				INTERNAL_SERVER_ERROR,
				BAD_REQUEST,
				NO_PRIVILAGES,
				BAD_CREDENTIALS,
				NOT_LOGGED,
				NO_SUCH_USER,
				NO_SUCH_NETWORK,
				BAD_DATA,

				BAD_SERVER_RESPONSE,
				SEND_ERROR,
				OTHER
			};

		}
		// Error from node and nodeclass
	}
}

#endif /* SP2P_TYPES_HPP */
