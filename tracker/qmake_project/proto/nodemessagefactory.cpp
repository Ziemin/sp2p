#include "nodemessagefactory.h"


using namespace std;
using namespace protocol;

shared_ptr<NodeMessage> errorMesage(NodeMessage_ResponseType resp) {
    shared_ptr<NodeMessage> request(new NodeMessage());
    request->set_response_type(resp);
    return request;
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

shared_ptr<NodeMessage> listNetworksResponse() {

}

shared_ptr<NodeMessage> listServersResponse() {

}


std::vector<boost::int8_t> sp2p::NodeMessageFactory::packNodeMessage(NodeMessage_ptr message)
{

}



