#ifndef DATA_H
#define DATA_H

#include <boost/asio.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <botan/botan.h>
#include <botan/symkey.h>
#include <botan/x509cert.h>

#include "node.hpp"
#include "network.hpp"
#include "sp2p_types.hpp"

namespace sp2p {
	namespace sercli {

		class Node;
		class Network;

		namespace serialization {

			class AbstractSink {

				public:
					virtual ~AbstractSink() = default;

					virtual AbstractSink& operator<<(const Node& node) = 0;
					virtual AbstractSink& operator<<(const Network& network) = 0;
					virtual AbstractSink& operator<<(const std::vector<types::NodeDescription>& nodes) = 0;
					virtual AbstractSink& operator<<(const std::vector<types::NetworkDescription>& networks) = 0;
					virtual AbstractSink& operator<<(const std::string& text) = 0;
			};

			class BasicSink : public AbstractSink {

				public:

					BasicSink(const std::string& dataFile);
					virtual ~BasicSink();

					virtual BasicSink& operator<<(const Node& node) override;
					virtual BasicSink& operator<<(const Network& network) override;
					virtual BasicSink& operator<<(const std::vector<types::NodeDescription>& nodes) override;
					virtual BasicSink& operator<<(const std::vector<types::NetworkDescription>& networks) override;
					virtual BasicSink& operator<<(const std::string& text) override;

				protected:

					std::ofstream os;
					boost::archive::text_oarchive oa;

			};

			class AbstractSource {

				public:
					virtual ~AbstractSource() = default;

					virtual AbstractSource& operator>>(Node& node) = 0;
					virtual AbstractSource& operator>>(Network& network) = 0;
					virtual AbstractSource& operator>>(std::vector<types::NodeDescription>& nodes) = 0;
					virtual AbstractSource& operator>>(std::vector<types::NetworkDescription>& networks) = 0;
					virtual AbstractSource& operator>>(std::string& text) = 0;
			};

			class BasicSource : public AbstractSource {

				public:
					BasicSource(const std::string& dataFile);
					virtual ~BasicSource();

					virtual BasicSource& operator>>(Node& node) override;
					virtual BasicSource& operator>>(Network& network) override;
					virtual BasicSource& operator>>(std::vector<types::NodeDescription>& nodes) override;
					virtual BasicSource& operator>>(std::vector<types::NetworkDescription>& networks) override;
					virtual BasicSource& operator>>(std::string& text) override;

				protected:
					std::ifstream is;
					boost::archive::text_iarchive ia;
			};

			class DataException : public std::exception {

				public:

					DataException(std::string message = "Node exception happened");
					virtual const char* what() const throw();

				private:
					std::string message;
            };

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

        } /* namespace serialization */


        /**
         * Abstract class representing data manager, whose duty is to save state
         * of networks and nodes
         */
        class DataManager {

            public:

                virtual ~DataManager();

                virtual std::shared_ptr<serialization::AbstractSink> getSink();
                virtual std::shared_ptr<serialization::AbstractSource> getSource();

                void setDataFile(std::string dataFile);

            protected:

                std::string dataFile;

        };


        namespace enc {

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
        }

    } /* namespace sercli */
} /* namespace sp2p */

#endif /* DATA_H */
