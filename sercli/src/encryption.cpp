#include "encryption.hpp"

#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <botan/hex.h>

namespace sp2p {
	namespace sercli {
		namespace enc {

            const std::string ALGORITHM = "AES-256";
            const std::string KEYALG = "PBKDF2(SHA-1)";
            const std::uint32_t PBKDF2_ITERATIONS = 8192;

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

        } /* namespace encryption */
    } /* namespace sercli */
} /* namespace sp2p */

