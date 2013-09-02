#define BOOST_TEST_MODULE node_connection_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

struct BeforeTest {
    
    BeforeTest() {
        global::init(10);
    }

    ~BeforeTest() {
        global::destroyAll();
    }

};

BOOST_FIXTURE_TEST_SUITE(node_connection_test, BeforeTest)

    BOOST_AUTO_TEST_CASE(simple_nodeconnection) {
        ConnectionManager<NodeRequest, NodeResponse> c_m;
        NodeConnection node_con(c_m);
    }


BOOST_AUTO_TEST_SUITE_END ( )

