#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <atomic>
#include <exception>
#include <sstream>
#include <botan/symkey.h>
#include <botan/botan.h>
#include "user.hpp"
#include "network.hpp"
#include "node.hpp"
#include "data.hpp"

namespace sp2p {
	namespace sercli {
		namespace enc {

			namespace serialization {

				class EncryptedSink : public sercli::serialization::AbstractSink {

					public:
						EncryptedSink(const std::string& dataFile, const std::string& password);
						virtual ~EncryptedSink();

						virtual EncryptedSink& operator<<(const Node& node) override;
						virtual EncryptedSink& operator<<(const Network& network) override;
						virtual EncryptedSink& operator<<(const std::vector<types::NodeDescription>& nodes) override;
						virtual EncryptedSink& operator<<(const std::vector<types::NetworkDescription>& networks) override;
						virtual EncryptedSink& operator<<(const std::string& text) override;

					protected:
						const std::string dataFile;
						const std::string password;
						std::string data;
						std::ostringstream oss;
						std::ofstream ofs;
						boost::archive::text_oarchive oa;
				};


				class EncryptedSource : public sercli::serialization::AbstractSource {

					public:
						EncryptedSource(const std::string& dataFile, const std::string& password);
						virtual ~EncryptedSource();

						virtual EncryptedSource& operator>>(Node& node) override;
						virtual EncryptedSource& operator>>(Network& network) override;
						virtual EncryptedSource& operator>>(std::vector<types::NodeDescription>& nodes) override;
						virtual EncryptedSource& operator>>(std::vector<types::NetworkDescription>& networks) override;
						virtual EncryptedSource& operator>>(std::string& text) override;

					protected:
						std::string data;
						std::istringstream* iss;
						boost::archive::text_iarchive* ia;
				};

			}


			class CryptDataManager : public DataManager {

				public:

					CryptDataManager(const std::string& password);
					~CryptDataManager() = default;

					virtual std::shared_ptr<sercli::serialization::AbstractSink> getSink() override;
					virtual std::shared_ptr<sercli::serialization::AbstractSource> getSource() override;

					void setPassword(std::string password);

				protected:

					std::string password;
			};


		} /* namespace encryption */
	} /* namespace sercli */
} /* namespace sp2p */

#endif /* ENCRYPTION_HPP */
