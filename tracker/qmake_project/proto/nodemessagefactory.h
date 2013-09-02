#ifndef NODEMESSAGEFACTORY_H
#define NODEMESSAGEFACTORY_H

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <boost/cstdint.hpp>

#include "types.hpp"

#include "sp2p_protocol.pb.h"


namespace sp2p {
namespace NodeMessageFactory {
using namespace protocol;
using namespace std;

NodeMessage_ptr errorMesage(NodeMessage_ResponseType resp);

NodeMessage_ptr registerResponse(string& userCertificate, string& nodeCertificate);

NodeMessage_ptr loginResponse(string& cookie);

NodeMessage_ptr loginResponse(string& cookie, string& certificate);

NodeMessage_ptr userInfoResponse(string& userCertificate);

NodeMessage_ptr updateServerResponse(int secondsToUpdate);

NodeMessage_ptr signKeyResponse(string& userCertificate);

NodeMessage_ptr listNetworksResponse();

NodeMessage_ptr ListServersResponse();

std::vector<boost::int8_t> packNodeMessage(NodeMessage_ptr message);

} // namespace NodeMessageFactory
} // namespace sp2p

#endif // NODEMESSAGEFACTORY_H
