#define BOOST_TEST_MODULE parser_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

BOOST_AUTO_TEST_SUITE(parser_test)

    BOOST_AUTO_TEST_CASE(simple_parse) {
        NodeResponse response;
        protocol::NodeMessage& message = response.getResponse();
        message.set_response_type(protocol::NodeMessage::OK);
        BOOST_TEST_MESSAGE("sth message");

        boost::asio::streambuf sb;
        std::ostream os(&sb);
        os << (std::uint32_t) message.ByteSize();
        message.SerializeToOstream(&os);
        std::istream is(&sb);
        char data[sb.size()];
        is.read(data, sb.size());

        NodeResponseParser parser;
        parse_result result = parser.parse(response, data, sb.size());

        BOOST_CHECK(result == parse_result::GOOD);
    }

    BOOST_AUTO_TEST_CASE(multi_step_parse) {

    }

    BOOST_AUTO_TEST_CASE(double_parse) {

    }

    BOOST_AUTO_TEST_CASE(parse_fail) {

    }

BOOST_AUTO_TEST_SUITE_END ( )

