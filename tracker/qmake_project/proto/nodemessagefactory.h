#ifndef NODEMESSAGEFACTORY_H
#define NODEMESSAGEFACTORY_H

#include <memory>
#include <string>
#include <iostream>

#include "sp2p_protocol.pb.h"

namespace NodeMessageFactory
{
using namespace protocol;
using namespace std;

shared_ptr<NodeMessage> errorMesage(NodeMessage_ResponseType resp);

shared_ptr<NodeMessage> registerResponse(string& userCertificate, string& nodeCertificate);

shared_ptr<NodeMessage> loginResponse(string& cookie);

shared_ptr<NodeMessage> loginResponse(string& cookie, string& certificate);

shared_ptr<NodeMessage> userInfoResponse(string& userCertificate);

shared_ptr<NodeMessage> updateServerResponse(int secondsToUpdate);

shared_ptr<NodeMessage> signKeyResponse(string& userCertificate);

shared_ptr<NodeMessage> listNetworksResponse();

shared_ptr<NodeMessage> listServersResponse();

}

#endif // NODEMESSAGEFACTORY_H
