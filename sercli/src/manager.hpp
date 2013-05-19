#ifndef MANAGER_H
#define MANAGER_H

#include <boost/asio.hpp>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include "data.hpp"
#include "node.hpp"

namespace sp2p {
	namespace sercli {

		class Manager {

			public:
				Manager(boost::asio::io_service& io_s, int threads_num, DataManager& data_manager);
				Manager(int threads_num, DataManager data_manager);

				void addNode(Node& node);

				Node& getNode(/* identyfikator - np .nazwa */) const;
				std::vector<node_ptr> getAllNodes() const;

				void save();

			private:
				DataManager &data_manager;
				boost::asio::io_service &io_s;

				std::map<std::string, node_ptr> nodes_map;

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* MANAGER_H */
