#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <map>
#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <atomic>
#include <exception>
#include <sstream>
#include <botan/botan.h>
#include <botan/symkey.h>
#include <botan/x509cert.h>
#include "user.hpp"

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
					void setPath(const std::string &filename);
					const std::string& getPath() const;
					const std::string& getFilename() const;

					virtual void save(const std::string& dir) = 0;
					virtual void save() = 0;
					virtual void load(const std::string& dir) = 0;
					virtual void load() = 0;

					virtual bool inMemory() = 0;
					bool inFile(const std::string& dir);

				protected:

					std::string name, filename, path = ".";
			};

			class PrivateKeyStore : public ElementStore {

				public:

					virtual ~PrivateKeyStore();
					PrivateKeyStore(Botan::Private_Key* = nullptr);

					virtual void save(const std::string& dir) override;
					virtual void save() override;
					virtual void load(const std::string& dir) override;
					virtual void load() override;

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
					virtual void save() override;
					virtual void load(const std::string& dir) override;
					virtual void load() override;
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
					virtual void save() override;
					virtual void load(const std::string& dir) override;
					virtual void load() override;
					void setCertificate(Botan::X509_Certificate* cert);
					Botan::X509_Certificate* getCertificate();
					virtual bool inMemory() override;

				private:

					Botan::X509_Certificate* cert = nullptr;

			};

			typedef std::shared_ptr<CertificateStore> cert_st_ptr;

		} /* namespace enc */
	} /* namespace sercli */
} /* namespace sp2p */

#endif /* ENCRYPTION_HPP */
