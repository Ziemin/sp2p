#define BOOST_TEST_MODULE node_connection_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include <vector>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

struct BeforeTest {
    BeforeTest() : init(5) { }
    global::Sp2pInitializer init;
};

BOOST_FIXTURE_TEST_SUITE(node_connection_test, BeforeTest)

    BOOST_AUTO_TEST_CASE(simple_nodeconnection) {
        ConnectionManager<NodeRequest, NodeResponse> c_m;
        std::vector<enc::cert_st_ptr> certs;
        NodeConnection node_con(c_m, certs);
    }


BOOST_AUTO_TEST_SUITE_END ( )

