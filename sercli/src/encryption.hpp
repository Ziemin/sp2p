#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <map>
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
#include <map>

namespace sp2p {
    namespace sercli {


		namespace enc {

			class EncryptionException : public std::exception {

				public:
					EncryptionException(std::string message);
					virtual const char* what() const throw();

				private:
					std::string message;
			};


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

			// utility functions
			Botan::Private_Key* generatePrivateKey(std::uint32_t bits);

			void savePrivateKeyToFile(const Botan::Private_Key& key, const std::string& filename, const std::string& password="");
			void savePublicKeyToFile(const Botan::Public_Key& key, const std::string& filename);

			Botan::Private_Key* getPrivateKeyFromFile(const std::string& filename, const std::string& password="");
			Botan::Public_Key* getPublicKeyFromFile(const std::string& filename);

			void saveCertificateToFile(const Botan::X509_Certificate& cert, const std::string& filename);
			Botan::X509_Certificate* getCertificateFromFile(const std::string& filename);

			class ElementStore {

				public:

					virtual ~ElementStore() = default;

					const std::string& getName() const;
					void setName(const std::string& name);
					void setFilename(const std::string &filename);
					const std::string& getFilename() const;

					virtual void save(const std::string& dir) = 0;
					virtual void load(const std::string& dir) = 0;

					virtual bool inMemory() = 0;
					bool inFile(const std::string& dir);

				protected:

					std::string name, filename;
			};

			class PrivateKeyStore : public ElementStore {

				public:

					virtual ~PrivateKeyStore();
					PrivateKeyStore(Botan::Private_Key* = nullptr);

					virtual void save(const std::string& dir) override;
					virtual void load(const std::string& dir) override;

					void setKey(Botan::Private_Key* key);
					Botan::Private_Key* getKey();
					virtual bool inMemory() override;
					void setPassword(const std::string& password);

				private:

					Botan::Private_Key* priv_key = nullptr;
					std::string password;

			};

			typedef std::shared_ptr<PrivateKeyStore> priv_st_ptr;

			class PublicKeyStore : public ElementStore {

				public:

					virtual ~PublicKeyStore();
					PublicKeyStore(Botan::Public_Key* = nullptr);

					virtual void save(const std::string& dir) override;
					virtual void load(const std::string& dir) override;
					void setKey(Botan::Public_Key* key);
					Botan::Public_Key* getKey();
					virtual bool inMemory() override;


				private:

					Botan::Public_Key* pub_key = nullptr;

			};

			typedef std::shared_ptr<PublicKeyStore> pub_st_ptr;

			class CertificateStore : public ElementStore {

				public:

					virtual ~CertificateStore();
					CertificateStore(Botan::X509_Certificate* = nullptr);

					virtual void save(const std::string& dir) override;
					virtual void load(const std::string& dir) override;
					void setCertificate(Botan::X509_Certificate* cert);
					Botan::X509_Certificate* getCertificate();
					virtual bool inMemory() override;

				private:

					Botan::X509_Certificate* cert = nullptr;

			};

			typedef std::shared_ptr<CertificateStore> cert_st_ptr;

			struct CryptContainer {

				std::map<std::string, priv_st_ptr> privateKeys;
				std::map<std::string, pub_st_ptr> publicKeys;
				std::map<std::string, cert_st_ptr> certificates;

				void save(const std::string& dir);
			};


		} /* namespace encryption */
	} /* namespace sercli */
} /* namespace sp2p */

#endif /* ENCRYPTION_HPP */
