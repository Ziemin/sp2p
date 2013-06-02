#ifndef DATA_H
#define DATA_H

#include <boost/asio.hpp>


namespace sp2p {
	namespace sercli {

		class Node;
		class Network;

		#include "node.hpp"
		#include "network.hpp"
		/**
		 * Abstract class representing data manager, whose duty is to save state
		 * of networks and nodes
		 */
		class DataManager {

			public:
				virtual void saveNode(const Node& node) = 0;
				virtual void saveNetwork(const Network& network) = 0;

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* DATA_H */
