#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace sp2p {
namespace tracker {
namespace consts {

const std::string CERT_PATH = "server.crt";
const std::string PRIV_KEY_PATH = "server.prv";
const std::string DH_FILE_PATH = "dh512.pem";

const int USER_TTL = 1000*60*30; // 30 min
const int SERVER_TTL = 1000*60*30; // 30 min
const std::uint64_t CERT_TTL = 1000*60*60*24*365L;

const std::string SERVER_ADDRESS = "127.0.0.1";
const std::string SERVER_PORT = "1234";
const int THREAT_NUMBER = 4;

}
}
}

#endif // CONSTANTS_H

