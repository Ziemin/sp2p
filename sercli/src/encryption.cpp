#include "encryption.hpp"

#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <botan/hex.h>
#include <botan/botan.h>
#include <botan/pem.h>
#include <botan/rsa.h>

namespace sp2p {
	namespace sercli {
		namespace enc {

            const std::string ALGORITHM = "AES-256";
            const std::string KEYALG = "PBKDF2(SHA-1)";
            const std::uint32_t PBKDF2_ITERATIONS = 8192;

            Botan::AutoSeeded_RNG rng;

            EncryptionException::EncryptionException(std::string message) : message(std::move(message)) { }

            const char* EncryptionException::what() const throw() {
                return message.c_str();
            }


            std::string base64encode(const Botan::SecureVector<Botan::byte>& vec) {

                Botan::Pipe pipe(new Botan::Base64_Encoder());
                pipe.process_msg(vec);
                return pipe.read_all_as_string();
            }

            Botan::SecureVector<Botan::byte> base64decode(const std::string& str) {
                Botan::Pipe pipe(new Botan::Base64_Decoder());
                pipe.process_msg(str);
                return pipe.read_all();
            }


			namespace serialization {

                using namespace sp2p::sercli::serialization;

                EncryptedSink::EncryptedSink(const std::string& dataFile, const std::string& password) 
                    : dataFile(dataFile),
                    password(password),
                    ofs(dataFile, std::ios::binary),
                    oa(oss) 
                {

                }

                EncryptedSink& EncryptedSink::operator<<(const Node& node) { 
                    oa << node; 
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const Network& network) {
                    oa << network; 
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const std::vector<types::NodeDescription>& nodes) {
                    oa << nodes;
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const std::vector<types::NetworkDescription>& networks) {
                    oa << networks;
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const std::string& text) {
                    oa << text; 
                    return *this;
                }

                EncryptedSink::~EncryptedSink() {
                    data = oss.str();

                    Botan::AutoSeeded_RNG rng;

                    const Botan::BlockCipher *cipher = Botan::global_state().algorithm_factory().prototype_block_cipher(ALGORITHM);
                    const std::uint32_t key_len = cipher->maximum_keylength();
                    const std::uint32_t iv_len = cipher->block_size();

                    std::shared_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf(KEYALG));
                    Botan::SecureVector<Botan::byte> salt(8);
                    rng.randomize(&salt[0], salt.size());

                    Botan::SymmetricKey symKey = pbkdf->derive_key(key_len, "BLK" + password,
                            &salt[0], salt.size(), PBKDF2_ITERATIONS);

                    Botan::InitializationVector iv = pbkdf->derive_key(iv_len, "IVL" + password,
                            &salt[0], salt.size(), PBKDF2_ITERATIONS);

                    Botan::SymmetricKey macKey = pbkdf->derive_key(16, "MAC" + password,
                            &salt[0], salt.size(), PBKDF2_ITERATIONS);

                    std::ofstream ofs(dataFile);
                    ofs << "-------- ENCRYPTED FILE --------" << std::endl;
                    ofs << ALGORITHM<< std::endl;

                    ofs << base64encode(salt) << std::endl;

                    Botan::Pipe encryptor(
                            new Botan::Fork(
                                new Botan::Chain(
                                    new Botan::MAC_Filter("HMAC(SHA-1)", macKey),
                                    new Botan::Base64_Encoder(true)),
                                new Botan::Chain(
                                    Botan::get_cipher(ALGORITHM + "/CBC", symKey, iv, Botan::ENCRYPTION),
                                    new Botan::Base64_Encoder(true))
                                )
                            );
                    encryptor.process_msg(data);
                    ofs << encryptor.read_all_as_string(0) << std::endl;
                    ofs << encryptor.read_all_as_string(1);
                }


                EncryptedSource::EncryptedSource(const std::string& dataFile, const std::string& password) {

                    std::ifstream ifs(dataFile);

                    std::string header;
                    std::string algo;
                    std::string salt_str;
                    std::string mac_str;
                    std::getline(ifs, header);
                    std::getline(ifs, algo);
                    std::getline(ifs, salt_str);
                    std::getline(ifs, mac_str);

                    const Botan::BlockCipher *cipher = Botan::global_state().algorithm_factory().prototype_block_cipher(ALGORITHM);
                    const std::uint32_t key_len = cipher->maximum_keylength();
                    const std::uint32_t iv_len = cipher->block_size();

                    std::shared_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf(KEYALG));
                    Botan::SecureVector<Botan::byte> salt = base64decode(salt_str);

                    Botan::SymmetricKey symKey = pbkdf->derive_key(key_len, "BLK" + password,
                            &salt[0], salt.size(), PBKDF2_ITERATIONS);

                    Botan::InitializationVector iv = pbkdf->derive_key(iv_len, "IVL" + password,
                            &salt[0], salt.size(), PBKDF2_ITERATIONS);

                    Botan::SymmetricKey macKey = pbkdf->derive_key(16, "MAC" + password,
                            &salt[0], salt.size(), PBKDF2_ITERATIONS);


                    Botan::Pipe decryptor(
                            new Botan::Base64_Decoder(),
                            Botan::get_cipher(ALGORITHM + "/CBC", symKey, iv, Botan::DECRYPTION),
                            new Botan::Fork(
                                0,
                                new Botan::Chain(
                                    new Botan::MAC_Filter("HMAC(SHA-1)", macKey),
                                    new Botan::Base64_Encoder(true))
                                )
                            );
                    decryptor.start_msg();
                    ifs >> decryptor;
                    decryptor.end_msg();

                    std::string res_mac = decryptor.read_all_as_string(1);
                    // .... 
                    data = decryptor.read_all_as_string(0);

                    iss = new std::istringstream(data);
                    ia = new boost::archive::text_iarchive(*iss);
                }


                EncryptedSource& EncryptedSource::operator>>(Node& node) {
                    *ia >> node; 
                    return *this;
                }

                EncryptedSource& EncryptedSource::operator>>(Network& network) {
                    *ia >> network; 
                    return *this;
                }

                EncryptedSource& EncryptedSource::operator>>(std::vector<types::NodeDescription>& nodes) {
                    *ia >> nodes;
                    return *this;
                }

                EncryptedSource& EncryptedSource::operator>>(std::vector<types::NetworkDescription>& networks) {
                    *ia >> networks;
                    return *this;
                }

                EncryptedSource& EncryptedSource::operator>>(std::string& text) { 
                    *ia >> text; 
                    return *this;
                }


                EncryptedSource::~EncryptedSource() {
                    if(ia != nullptr) delete ia;
                    if(iss != nullptr) delete iss;
                }


            } /* namespace serialization */


            CryptDataManager::CryptDataManager(const std::string& password) : password(password) { }

            void CryptDataManager::setPassword(std::string password) {
                this->password = std::move(password);
            }

            std::shared_ptr<sercli::serialization::AbstractSink> CryptDataManager::getSink() {
                if(dataFile.size() == 0) throw sercli::serialization::DataException("no file given");
                return std::shared_ptr<sercli::serialization::AbstractSink>(new serialization::EncryptedSink(dataFile, password));
            }

            std::shared_ptr<sercli::serialization::AbstractSource> CryptDataManager::getSource() {
                if(dataFile.size() == 0) throw sercli::serialization::DataException("no file given");

                return std::shared_ptr<sercli::serialization::AbstractSource>(new serialization::EncryptedSource(dataFile, password));
            }

            // ---------------------- utility functions -----------------------------------

			Botan::Private_Key* generatePrivateKey(std::uint32_t bits) {
                if(bits < 1024 || bits > 16384) throw EncryptionException("Invalid bits size");
                return new Botan::RSA_PrivateKey(rng, bits);
            }

			void savePrivateKeyToFile(const Botan::Private_Key& key, const std::string& filename, const std::string& password) {
                std::ofstream ofs(filename);
                if(password.size() > 0) ofs << Botan::PKCS8::PEM_encode(key, rng, password);
                else ofs << Botan::PKCS8::PEM_encode(key);
            }

			Botan::Private_Key* getPrivateKeyFromFile(const std::string& filename, const std::string& password) {
                try {
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

            void PrivateKeyStore::load(const std::string& dir) {
                priv_key = getPrivateKeyFromFile(dir + "/" + filename, password);
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
                if(priv_key == nullptr && inFile()) load();
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

            void PublicKeyStore::load(const std::string& dir) {
                pub_key = getPublicKeyFromFile(dir + "/" + filename);
            }

            bool PublicKeyStore::inMemory() {
                return pub_key != nullptr;
            }

            void PublicKeyStore::setKey(Botan::Public_Key* key) {
                if(pub_key != nullptr) delete pub_key;
                pub_key = key;
            }

            Botan::Public_Key* PublicKeyStore::getKey() {
                if(pub_key == nullptr && inFile()) load();
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

            void CertificateStore::load(const std::string& dir) {
                cert = getCertificateFromFile(dir + "/" + filename);
            }

            bool CertificateStore::inMemory() {
                return cert != nullptr;
            }

            void CertificateStore::setCertificate(Botan::X509_Certificate* cert) {
                if(this->cert != nullptr) delete this->cert;
                this->cert = cert;
            }

            Botan::X509_Certificate* CertificateStore::getCertificate() {
                if(cert == nullptr && inFile()) load();
                return cert;
            }

            // Crypt Container --------------------------------------------------------------
            
            void CryptContainer::save(const std::string& dir) {
                for(auto& priv_key_pair: privateKeys) priv_key_pair.second->save(dir);
                for(auto& pub_key_pair: publicKeys) pub_key_pair.second->save(dir);
                for(auto& cert_pair: certificates) cert_pair.second->save(dir);
            }

        } /* namespace encryption */
    } /* namespace sercli */
} /* namespace sp2p */

