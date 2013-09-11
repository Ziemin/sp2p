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

			namespace serialization {

                using namespace sp2p::sercli::serialization;

                EncryptedSink::EncryptedSink(const std::string& dataFile, const std::string& password) 
                    : dataFile(dataFile),
                    password(password),
                    ofs(dataFile, std::ios::binary),
                    oa(oss) 
                {

                    std::cout << " const " << std::endl;
                }

                EncryptedSink& EncryptedSink::operator<<(const Node& node) { 
                    std::cout << " node1 " << std::endl;
                    oa << node; 
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const Network& network) {
                    std::cout << " net1 " << std::endl;
                    oa << network; 
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const std::vector<types::NodeDescription>& nodes) {
                    std::cout << " node " << std::endl;
                    oa << nodes;
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const std::vector<types::NetworkDescription>& networks) {
                    std::cout << " net " << std::endl;
                    oa << networks;
                    return *this;
                }

                EncryptedSink& EncryptedSink::operator<<(const std::string& text) {
                    std::cout << " text " << std::endl;
                    oa << text; 
                    return *this;
                }

                EncryptedSink::~EncryptedSink() {
                    std::cout << "destructor" << std::endl;
                    data = oss.str();

                    Botan::AutoSeeded_RNG rng;
                    Botan::InitializationVector iv(rng, 16);

                    std::string encoded_pass;
                    Botan::Pipe hasher(new Botan::Hash_Filter("SHA-256"), new Botan::Hex_Encoder);
                    hasher.process_msg(password);
                    encoded_pass = hasher.read_all_as_string(0);
                    std::cout << encoded_pass << std::endl;

                    Botan::SymmetricKey symKey(encoded_pass);
                    std::ofstream ofs(dataFile, std::ios::binary);
                    Botan::Pipe encryptor(
                            Botan::get_cipher("AES-256/CBC", symKey, iv, Botan::ENCRYPTION));
                    std::cout << data << std::endl;
                    encryptor.process_msg(data);
                    ofs << encryptor;
                }


                EncryptedSource::EncryptedSource(const std::string& dataFile, const std::string& password) {

                    Botan::AutoSeeded_RNG rng;
                    Botan::InitializationVector iv(rng, 16);

                    std::string encoded_pass;
                    Botan::Pipe hasher(new Botan::Hash_Filter("SHA-256"), new Botan::Hex_Encoder);
                    hasher.process_msg(password);
                    encoded_pass = hasher.read_all_as_string(0);
                    std::cout << encoded_pass << std::endl;

                    Botan::SymmetricKey key(encoded_pass);

                    Botan::Pipe decryptor(Botan::get_cipher("AES-256/CBC", key, iv, Botan::DECRYPTION));
                    std::ifstream ifs(dataFile, std::ios::binary);
                    decryptor.start_msg();
                    ifs >> decryptor;
                    decryptor.end_msg();
                    std::cout << "after process" << std::endl;
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
                    if(iss != nullptr) delete iss;
                    if(ia != nullptr) delete ia;
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

