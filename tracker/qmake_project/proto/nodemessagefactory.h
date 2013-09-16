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

/**
 * @brief errorMesage - produces message containing only status
 * @param resp - protocol::NodeMessage_ResponseType with the status
 * @return
 */
NodeMessage_ptr errorMesage(NodeMessage_ResponseType resp);

/**
 * @brief badRequestMessage - like errorMesage() but always produces BAD_REQUEST
 * @return
 */
NodeMessage_ptr badRequestMessage();

/**
 * @brief registerResponse - produces response to register message
 * @param userCertificate
 * @param nodeCertificate
 * @return
 */
NodeMessage_ptr registerResponse(string& userCertificate, string& nodeCertificate);

/**
 * @brief loginResponse - produces response to login message
 * @param cookie
 * @return
 */
NodeMessage_ptr loginResponse(string& cookie);

/**
 * @brief loginResponse - producs response to login message
 * @param cookie
 * @param certificate
 * @return
 */
NodeMessage_ptr loginResponse(string& cookie, string& certificate);

/**
 * @brief userInfoResponse - produces response to user_info message
 * @param userCertificate
 * @return
 */
NodeMessage_ptr userInfoResponse(string& userCertificate);

/**
 * @brief updateServerResponse - produces response to update_server
 * @param secondsToUpdate
 * @return
 */
NodeMessage_ptr updateServerResponse(int secondsToUpdate);

/**
 * @brief signKeyResponse  - produces response to sign_key request
 * @param userCertificate
 * @return
 */
NodeMessage_ptr signKeyResponse(string& userCertificate);

/**
 * @brief listNetworksResponse - produces response to list_networks message
 * @param
 * @return
 */
NodeMessage_ptr listNetworksResponse(shared_ptr<list<utils::ProtoNetwork> > L);

/**
 * @brief listMyNetworksResponse - produces response to list_my_networks message
 * @param
 * @return
 */
NodeMessage_ptr listMyNetworksResponse(shared_ptr<list<utils::ProtoNetwork> > L);

/**
 * @brief listServersResponse - produces response to slist_servers
 * @param L
 * @return
 */
NodeMessage_ptr listServersResponse(shared_ptr<list<utils::ProtoServer> > L);



} // namespace NodeMessageFactory
} // namespace tracker
} // namespace sp2p

#endif // NODEMESSAGEFACTORY_H
