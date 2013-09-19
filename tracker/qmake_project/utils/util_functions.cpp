#include "util_functions.h"
#include "../constants.h"

#include <algorithm>
#include <time.h>
#include <iostream>
#include <boost/array.hpp>
#include <boost/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <chrono>

namespace sp2p{
namespace tracker{
namespace utils{

Botan::X509_CA *certificateAuthoritie_;
std::string nodeCert;

std::string getRandomString(int len) {
    auto randchar = []() -> char
    {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
     };
    std::string str(len, 0);
    std::generate_n( str.begin(), len, randchar );
    return str;
}

long long currTimeInMS()
{
    auto time = std::chrono::system_clock::now();

    auto since_epoch = time.time_since_epoch();

    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>( since_epoch );

    auto now = millis.count(); // just like java (new Date()).getTime();

    return now;
}

std::string toString(pqxx::tuple::reference ref)
{
    std::stringstream stream;
    stream << ref;
    std::string ret;
    stream >> ret;
    return ret;
}

bool toBool(pqxx::tuple::reference ref)
{
    std::string s = toString(ref);
    if(s == "t") return true;
    return false;
}

int toInt(pqxx::tuple::reference ref)
{
    std::string s = toString(ref);
    return std::stoi(s);
}

std::string getPasswordHash(std::string &password)
{
    Botan::AutoSeeded_RNG rng;

    return Botan::generate_bcrypt(password, rng);
}


void init_ca(const std::string &certPath, const std::string &privateKeyPath)
{
    // get provate key pass

    std::string privPass;
    std::cout << "CA private key password: ";
    std::cin >> privPass;

    // create CA class object

    Botan::X509_Certificate cert(certPath);
    Botan::AutoSeeded_RNG rng;
    Botan::Private_Key* pKey = Botan::PKCS8::load_key(privateKeyPath, rng, privPass);
    certificateAuthoritie_ = new Botan::X509_CA(cert, *pKey, "SHA-256");

    // load server public key to RAM

    std::ifstream ifs(certPath);
    std::string str((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
    nodeCert = cert.PEM_encode();

}

std::string signCertyficate(std::string &request)
{
    try {
        Botan::DataSource_Memory dataSource(request);
        Botan::PKCS10_Request pkcs10Request(dataSource);
        Botan::AutoSeeded_RNG rng;
        std::cout << currTimeInMS() << std::endl;
        Botan::X509_Time notBefore(currTimeInMS()/1000);
        Botan::X509_Time notAfter((currTimeInMS() + consts::CERT_TTL)/1000);
        auto signed_ = certificateAuthoritie_->sign_request(pkcs10Request, rng, notBefore, notAfter);
        return signed_.PEM_encode();
    } catch(std::exception e) {
#ifdef DEBUG_LOGGING
        BOOST_LOG_TRIVIAL(debug) << "Invalid argument in certificate signing: " << e.what();
#endif
        return "";
    }
}

std::string &getNodeCert()
{
    return nodeCert;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p
