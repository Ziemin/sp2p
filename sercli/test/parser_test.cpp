#define BOOST_TEST_MODULE parser_test

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

BOOST_AUTO_TEST_SUITE(parser_test)

    BOOST_AUTO_TEST_CASE(simple_parse) {
        NodeResponse response;
        protocol::NodeMessage& message = response.getResponse();
        message.set_response_type(protocol::NodeMessage::OK);

        boost::asio::streambuf sb;
        std::ostream os(&sb);
        std::uint32_t message_size = message.ByteSize();
        os.write(reinterpret_cast<const char*>(&message_size), sizeof(std::uint32_t));
        BOOST_CHECK(sb.size() == sizeof(std::uint32_t));

        BOOST_TEST_MESSAGE("Message size: " << message.ByteSize());

        message.SerializeToOstream(&os);
        os.flush();

        std::istream is(&sb);
        char data[sb.size()];
        BOOST_TEST_MESSAGE("Buffer size: " << sb.size());

        int length = sb.size();
        is.read(data, length);

        NodeResponseParser parser;
        parse_result result = parser.parse(response, data, length);

        BOOST_CHECK(result == parse_result::GOOD);
        BOOST_CHECK(message.response_type() == protocol::NodeMessage::OK);
    }

    BOOST_AUTO_TEST_CASE(protobuf_test) {
        NodeResponse response;
        protocol::NodeMessage& message = response.getResponse();
        message.set_response_type(protocol::NodeMessage::OK);

        boost::asio::streambuf sb;
        std::ostream os(&sb);
        message.SerializeToOstream(&os);
        std::istream is(&sb);
        BOOST_CHECK(message.ParseFromIstream(&is) == true);
        BOOST_CHECK(sb.size() == 0);
    }
    
    BOOST_AUTO_TEST_CASE(messsage_size_test) {
        std::uint32_t size = 10;

        boost::asio::streambuf sb;
        std::ostream os(&sb);
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));
        os.flush();

        std::istream is(&sb);
        std::uint32_t result_size;
        is.read(reinterpret_cast<char*>(&result_size), sizeof(result_size));
        BOOST_TEST_MESSAGE("Input size: " << size);
        BOOST_TEST_MESSAGE("Result size: " << result_size);
        BOOST_CHECK(size == result_size);
    }

    BOOST_AUTO_TEST_CASE(double_parse) {
        NodeResponse response;
        protocol::NodeMessage& message = response.getResponse();
        message.set_response_type(protocol::NodeMessage::OK);
        protocol::NodeMessage::Register* register_message = new protocol::NodeMessage::Register();
        std::string node_cert = "node_cert";
        std::string user_cert = "user_cert";
        register_message->set_node_certificate(node_cert);
        register_message->set_user_certificate(user_cert);

        message.set_allocated_register_response(register_message);

        boost::asio::streambuf sb;
        std::ostream os(&sb);
        std::uint32_t size = message.ByteSize();
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));
        message.SerializeToOstream(&os);
        os.flush();
        int data_size = sb.size();
        char data[data_size];
        std::istream is(&sb);
        is.read(data, data_size);

        int first_part = data_size/2, second_part = data_size-first_part;

        response = NodeResponse(); // new response;
        NodeResponseParser parser;
        BOOST_CHECK(parser.parse(response, data, first_part) == parse_result::INDETERMINATE);
        BOOST_CHECK(parser.parse(response, data+first_part, second_part) == parse_result::GOOD);
        BOOST_CHECK(message.register_response().user_certificate()== user_cert);
        BOOST_CHECK(message.register_response().node_certificate() == node_cert);
    }

    BOOST_AUTO_TEST_CASE(parse_fail) {
        NodeResponse response;
        protocol::NodeMessage& message = response.getResponse();
        message.set_response_type(protocol::NodeMessage::OK);
        protocol::NodeMessage::Register* register_message = new protocol::NodeMessage::Register();
        std::string node_cert = "node_cert";
        std::string user_cert = "user_cert";
        register_message->set_node_certificate(node_cert);
        register_message->set_user_certificate(user_cert);

        message.set_allocated_register_response(register_message);

        boost::asio::streambuf sb;
        std::ostream os(&sb);
        std::uint32_t size = message.ByteSize()/2;
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));
        message.SerializeToOstream(&os);
        os.flush();
        int data_size = sb.size();
        char data[data_size];
        std::istream is(&sb);
        is.read(data, data_size);

        response = NodeResponse(); // new response;
        NodeResponseParser parser;
        BOOST_CHECK(parser.parse(response, data, size+sizeof(size)) == parse_result::BAD);
    }

    BOOST_AUTO_TEST_CASE(parser_factory) {
        NodeParserFactory factory;
        parser_ptr<NodeResponse> parser = factory.getParser();
        BOOST_CHECK(parser != nullptr);
    }

BOOST_AUTO_TEST_SUITE_END ( )

