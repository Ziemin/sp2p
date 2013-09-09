#ifndef NODEMESSAGEFACTORY_H
#define NODEMESSAGEFACTORY_H

#include <memory>
#include <string>
#include <iostream>
#include <list>
#include <boost/cstdint.hpp>

#include "types.hpp"
#include "utils/proto_network.hpp"
#include "utils/proto_server.hpp"
#include "sp2p_protocol.pb.h"


namespace sp2p {
namespace tracker {
namespace NodeMessageFactory {
using namespace protocol;
using namespace std;

NodeMessage_ptr errorMesage(NodeMessage_ResponseType resp);

NodeMessage_ptr badRequestMessage();

NodeMessage_ptr registerResponse(string& userCertificate, string& nodeCertificate);

NodeMessage_ptr loginResponse(string& cookie);

NodeMessage_ptr loginResponse(string& cookie, string& certificate);

NodeMessage_ptr userInfoResponse(string& userCertificate);

NodeMessage_ptr updateServerResponse(int secondsToUpdate);

NodeMessage_ptr signKeyResponse(string& userCertificate);

NodeMessage_ptr listNetworksResponse(shared_ptr<list<utils::ProtoNetwork> > L);

NodeMessage_ptr listMyNetworksResponse(shared_ptr<list<utils::ProtoNetwork> > L);

NodeMessage_ptr listServersResponse(shared_ptr<list<utils::ProtoServer> > L);



} // namespace NodeMessageFactory
} // namespace tracker
} // namespace sp2p

#endif // NODEMESSAGEFACTORY_H
