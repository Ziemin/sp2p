#ifndef DATA_H
#define DATA_H

#include <boost/asio.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "node.hpp"
#include "network.hpp"

namespace sp2p {
	namespace sercli {

		class Node;
		class Network;

		namespace serialization {

			class AbstractSink {

				public:
					virtual ~AbstractSink() = default;

					virtual AbstractSink& operator<<(const Node& node) = 0;
					virtual AbstractSink& operator<<(const Network& node) = 0;
			};

			class BasicSink : AbstractSink {

				public:

					BasicSink(const std::string& dataFile);
					virtual ~BasicSink();

					virtual AbstractSink& operator<<(const Node& node);
					virtual AbstractSink& operator<<(const Network& node);

				protected:

					boost::archive::text_oarchive oa;

			};

			class AbstractSource {

				public:
					virtual ~AbstractSource() = default;

					virtual AbstractSource& operator>>(Node& node) = 0;
					virtual AbstractSource& operator>>(Network& node) = 0;
			};

			class BasicSource : AbstractSource {

				public:
					BasicSource(const std::string& dataFile);
					virtual ~BasicSource();

					virtual AbstractSource& operator>>(Node& node);
					virtual AbstractSource& operator>>(Network& node);

				protected:

					boost::archive::text_iarchive ia;
			};

		} /* namespace serialization */


		/**
		 * Abstract class representing data manager, whose duty is to save state
		 * of networks and nodes
		 */
		class BasicDataManager {

			public:

				virtual ~BasicDataManager();

				virtual std::shared_ptr<serialization::AbstractSink> getSink();
				virtual std::shared_ptr<serialization::AbstractSource> getSource();

				void setDataFile(std::string dataFile);

			private:

				std::string dataFile;

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* DATA_H */
