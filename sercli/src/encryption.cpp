#include "encryption.hpp"

#include <botan/hex.h>
#include <botan/botan.h>
#include <botan/pem.h>
#include <botan/rsa.h>
#include <fstream>
#include <iostream>

namespace sp2p {
	namespace sercli {
		namespace enc {



            EncryptionException::EncryptionException(std::string message) : message(std::move(message)) { }

            const char* EncryptionException::what() const throw() {
                return message.c_str();
            }

            // ---------------------- utility functions -----------------------------------

			Botan::Private_Key* generatePrivateKey(std::uint32_t bits) {
                if(bits < 1024 || bits > 16384) throw EncryptionException("Invalid bits size");
                Botan::AutoSeeded_RNG rng;
                return new Botan::RSA_PrivateKey(rng, bits);
            }

			void savePrivateKeyToFile(const Botan::Private_Key& key, const std::string& filename, const std::string& password) {
                Botan::AutoSeeded_RNG rng;
                std::ofstream ofs(filename);
                if(password.size() > 0) ofs << Botan::PKCS8::PEM_encode(key, rng, password);
                else ofs << Botan::PKCS8::PEM_encode(key);
            }

			Botan::Private_Key* getPrivateKeyFromFile(const std::string& filename, const std::string& password) {
                try {
                    Botan::AutoSeeded_RNG rng;
                    Botan::Private_Key* key = Botan::PKCS8::load_key(filename, rng, password);
                    return key;
                } catch(std::exception& e) {
                    throw EncryptionException(e.what());
                }
            }

			void savePublicKeyToFile(const Botan::Public_Key& key, const std::string& filename) {
                std::ofstream ofs(filename);
                ofs << Botan::X509::PEM_encode(key);
            }

			Botan::Public_Key* getPublicKeyFromFile(const std::string& filename) {
                try {
                    Botan::Public_Key* key = Botan::X509::load_key(filename);
                    return key;
                } catch(std::exception& e) {
                    throw EncryptionException(e.what());
                }
            }

			void saveCertificateToFile(const Botan::X509_Certificate& cert, const std::string& filename) {
                std::ofstream ofs(filename);
                ofs << cert.PEM_encode();
            }

            Botan::X509_Certificate* getCertificateFromFile(const std::string& filename) {
                try {
                    Botan::X509_Certificate* cert = new Botan::X509_Certificate(filename);
                    return cert;
                } catch(std::exception& e) {
                    throw EncryptionException(e.what());
                }
            }

            // --------------------------------- Stores ------------------------------------
            
            const std::string& ElementStore::getName() const {
                return name;
            }

            const std::string& ElementStore::getFilename() const {
                return filename;
            }

            void ElementStore::setName(const std::string& name) {
                this->name = name;
            } 

            void ElementStore::setFilename(const std::string& filename) {
                this->filename = filename;
            }

            const std::string& ElementStore::getPath() const {
                return path;
            }

            void ElementStore::setPath(const std::string& path) {
                this->path = path;
            }
            
            bool ElementStore::inFile(const std::string& dir) {
                if(std::ifstream(dir + "/" + filename)) return true;
                else return false;
            }


            // Private Key Store ------------------------------------------------------------
            PrivateKeyStore::PrivateKeyStore(Botan::Private_Key* key) : priv_key(key), password("") { }

            PrivateKeyStore::~PrivateKeyStore() {
                if(priv_key != nullptr) delete priv_key;
            }

            void PrivateKeyStore::save(const std::string& dir) {
                savePrivateKeyToFile(*priv_key, dir + "/" + filename, password);
            }

            void PrivateKeyStore::save() {
                save(path);
            }

            void PrivateKeyStore::load(const std::string& dir) {
                priv_key = getPrivateKeyFromFile(dir + "/" + filename, password);
            }

            void PrivateKeyStore::load() {
                return load(path);
            }

            bool PrivateKeyStore::inMemory() {
                return priv_key != nullptr;
            }

            void PrivateKeyStore::setKey(Botan::Private_Key* key) {
                if(priv_key != nullptr) delete priv_key;
                priv_key = key;
            }

            void PrivateKeyStore::setPassword(const std::string& password) {
                this->password = password;
            }

            Botan::Private_Key* PrivateKeyStore::getKey() {
                if(priv_key == nullptr && inFile(path)) load();
                return priv_key;
            }


            //Public Key Store ------------------------------------------------------------
            PublicKeyStore::PublicKeyStore(Botan::Public_Key* key) : pub_key(key) { }

            PublicKeyStore::~PublicKeyStore() {
                if(pub_key != nullptr) delete pub_key;
            }

            void PublicKeyStore::save(const std::string& dir) {
                savePublicKeyToFile(*pub_key, dir + "/" + filename);
            }

            void PublicKeyStore::save() {
                save(path);
            }

            void PublicKeyStore::load(const std::string& dir) {
                pub_key = getPublicKeyFromFile(dir + "/" + filename);
            }

            void PublicKeyStore::load() {
                return load(path);
            }

            bool PublicKeyStore::inMemory() {
                return pub_key != nullptr;
            }

            void PublicKeyStore::setKey(Botan::Public_Key* key) {
                if(pub_key != nullptr) delete pub_key;
                pub_key = key;
            }

            Botan::Public_Key* PublicKeyStore::getKey() {
                if(pub_key == nullptr && inFile(path)) load();
                return pub_key;
            }

            //Certificate Store ------------------------------------------------------------
            CertificateStore::CertificateStore(Botan::X509_Certificate* cert) : cert(cert) { }

            CertificateStore::~CertificateStore() {
                if(cert != nullptr) delete cert;
            }

            void CertificateStore::save(const std::string& dir) {
                saveCertificateToFile(*cert, dir + "/" + filename);
            }

            void CertificateStore::save() {
                save(path);
            }

            void CertificateStore::load(const std::string& dir) {
                cert = getCertificateFromFile(dir + "/" + filename);
            }

            void CertificateStore::load() {
                return load(path);
            }

            bool CertificateStore::inMemory() {
                return cert != nullptr;
            }

            void CertificateStore::setCertificate(Botan::X509_Certificate* cert) {
                if(this->cert != nullptr) delete this->cert;
                this->cert = cert;
            }

            Botan::X509_Certificate* CertificateStore::getCertificate() {
                if(cert == nullptr && inFile(path)) load();
                return cert;
            }

        } /* namespace encryption */
    } /* namespace sercli */
} /* namespace sp2p */

