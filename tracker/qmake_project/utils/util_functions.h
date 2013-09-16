#ifndef UTILSSP2P_H
#define UTILSSP2P_H

#include <string>
#include <pqxx/result>
#include <boost/asio/streambuf.hpp>
#include <botan/botan.h>
#include <botan/bcrypt.h>
#include <botan/init.h>
#include <botan/auto_rng.h>
#include <botan/x509self.h>
#include <botan/rsa.h>
#include <botan/dsa.h>
#include <botan/data_src.h>
#include <botan/pkcs10.h>
#include <botan/x509_ca.h>

namespace sp2p{
namespace tracker{
namespace utils{

/**
 * @brief getRandomString - generates random string of digits, small and capital letters
 * @param len - length of generated string
 * @return
 */
std::string getRandomString(int len);

/**
 * @brief currTimeInMS
 * @return current time in miliseconds
 */
long long currTimeInMS();

/**
 * @brief toString - pqxx reference to std::string
 * @return
 */
std::string toString(pqxx::tuple::reference);

/**
 * @brief toBool - pqxx reference to std bool
 * @return
 */
bool toBool(pqxx::tuple::reference);

/**
 * @brief toInt - pqxx reference to std int
 * @return
 */
int toInt(pqxx::tuple::reference);

/**
 * @brief getPasswordHash - hashes password
 * @param password
 * @return
 */
std::string getPasswordHash(std::string &password);

/**
 * @brief init_ca - initialize certificate authority. Needed to be done before using
 * signCertyficate
 * @param certPath - path to CA certyficate
 * @param privateKeyPath - path to CA private key
 */
void init_ca(const std::string &certPath, const std::string &privateKeyPath);

/**
 * @brief signCertyficate
 * @param request
 * @return signed certificate or empty string if error occured
 */
std::string signCertyficate(std::string &request);

/**
 * @brief getNodeCert
 * @return node certyficate
 */
std::string &getNodeCert();

/**
 * @brief getNumFromBuff - parses numSize bytes from buff to std::int64_t
 * @param buff
 * @param numSize
 * @return
 */
template <std::size_t N> std::int64_t getNumFromBuff(boost::array<char, N> &buff, int numSize)
{
    std::int64_t ret = 0;
    int tmp = 1;
    for(int i = 0; i < numSize; ++i) {
        char currChar = buff.at(i);
        ret += tmp * ((unsigned int)(unsigned char)currChar);
        tmp <<= 8;
    }
    return ret;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // UTILS_H
