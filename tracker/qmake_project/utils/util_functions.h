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

std::string getRandomString(int len);

long long currTimeInMS();
std::string toString(pqxx::tuple::reference);
bool toBool(pqxx::tuple::reference);
int toInt(pqxx::tuple::reference);
std::string getPasswordHash(std::string &password);

void init_ca(const std::string &certPath, const std::string &privateKeyPath);

std::string signCertyficate(std::string &reques);

std::string &getNodeCert();

template <std::size_t N>
std::int64_t getNumFromBuff(boost::array<char, N> &buff, int numSize)
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
