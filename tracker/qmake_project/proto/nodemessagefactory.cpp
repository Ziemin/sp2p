#include "nodemessagefactory.h"

namespace sp2p {
namespace tracker {
namespace NodeMessageFactory {

using namespace std;
using namespace protocol;

shared_ptr<NodeMessage> errorMesage(NodeMessage_ResponseType resp) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(resp);
    return request;
}

NodeMessage_ptr badRequestMessage() {
    return errorMesage(NodeMessage::BAD_REQUEST);
}

shared_ptr<NodeMessage> registerResponse(string& userCertificate, string& nodeCertificate) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);
    NodeMessage::Register *register_message = new NodeMessage::Register();

    register_message->set_user_certificate(userCertificate);
    register_message->set_node_certificate(nodeCertificate);

    request->set_allocated_register_response(register_message);
    return request;
}

shared_ptr<NodeMessage> loginResponse(string& cookie) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);
    NodeMessage::Login *login_message = new NodeMessage::Login();

    login_message->set_cookie(cookie);

    request->set_allocated_login_response(login_message);

    return request;
}

shared_ptr<NodeMessage> loginResponse(string& cookie, string& certificate) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);
    NodeMessage::Login *login_message = new NodeMessage::Login();

    login_message->set_cookie(cookie);
    login_message->set_node_certificate(certificate);

    request->set_allocated_login_response(login_message);

    return request;
}

shared_ptr<NodeMessage> userInfoResponse(string& userCertificate) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);
    auto user_info_message = new NodeMessage::UserInfo();

    user_info_message->set_user_public_key(userCertificate);

    request->set_allocated_user_info_response(user_info_message);

    return request;
}

shared_ptr<NodeMessage> updateServerResponse(int secondsToUpdate) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);

    NodeMessage::UpdateServer* update_server_response = new NodeMessage::UpdateServer();

    update_server_response->set_seconds_to_update(secondsToUpdate);

    request->set_allocated_update_server_response(update_server_response);

    return request;
}

shared_ptr<NodeMessage> signKeyResponse(string& userCertificate) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);

    NodeMessage::SignKey* sign_key_response = new NodeMessage::SignKey();

    sign_key_response->set_user_certificate(userCertificate);

    request->set_allocated_sign_key_response(sign_key_response);

    return request;
}

shared_ptr<NodeMessage> listNetworksResponse(shared_ptr<list<utils::ProtoNetwork> > L) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);

    NodeMessage::ListNetworks *list_networks_response = new NodeMessage::ListNetworks();

    for(auto i = L->begin(); i != L->end(); ++i) {
        NodeMessage::ListNetworks::Network *currentNet = list_networks_response->add_network_list();
        currentNet->set_name(i->getName());
        currentNet->set_creator_email(i->getCreatorEmail());
        currentNet->set_creator_name(i->getOwnerName());
        if(i->getPublicity()) {
            currentNet->set_access_rights(ClientMessage::CreateNetwork::PUBLIC);
        }
        else {
            currentNet->set_access_rights(ClientMessage::CreateNetwork::PRIVATE);
        }
        if(i->getParticipable()) {
            currentNet->set_participation_rights(ClientMessage::CreateNetwork::CLIENT_SERVER);
        }
        else {
            currentNet->set_participation_rights(ClientMessage::CreateNetwork::CLIENT_ONLY);
        }
        currentNet->set_protocol_name(i->getProtocolName());
    }

    request->set_allocated_list_networks_response(list_networks_response);

    return request;

}

NodeMessage_ptr listMyNetworksResponse(shared_ptr<list<utils::ProtoNetwork> > L) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);

    NodeMessage::ListMyNetworks *list_networks_response = new NodeMessage::ListMyNetworks();

    for(auto i = L->begin(); i != L->end(); ++i) {
        NodeMessage::ListNetworks::Network *currentNet = list_networks_response->add_network_list();
        currentNet->set_name(i->getName());
        currentNet->set_creator_email(i->getCreatorEmail());
        currentNet->set_creator_name(i->getOwnerName());
        if(i->getPublicity()) {
            currentNet->set_access_rights(ClientMessage::CreateNetwork::PUBLIC);
        }
        else {
            currentNet->set_access_rights(ClientMessage::CreateNetwork::PRIVATE);
        }
        if(i->getParticipable()) {
            currentNet->set_participation_rights(ClientMessage::CreateNetwork::CLIENT_SERVER);
        }
        else {
            currentNet->set_participation_rights(ClientMessage::CreateNetwork::CLIENT_ONLY);
        }
        currentNet->set_protocol_name(i->getProtocolName());
    }

    request->set_allocated_list_my_networks_response(list_networks_response);

    return request;
}

shared_ptr<NodeMessage> listServersResponse(shared_ptr<list<utils::ProtoServer> > L) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(NodeMessage_ResponseType_OK);

    NodeMessage::ListServers *list_servers_response = new NodeMessage::ListServers();

    for(auto i = L->begin(); i != L->end(); ++i) {
        NodeMessage::ListServers::Server *currentServer = list_servers_response->add_list_servers();
        currentServer->set_ip_address(i->getIp());
        currentServer->set_port_number(i->getPort());
        currentServer->set_username(i->getNetworkName());
    }

    request->set_allocated_list_servers_response(list_servers_response);

    return request;

}




} // namespace NodeMessageFactory
} // namespace tracker
} // namespace sp2p
