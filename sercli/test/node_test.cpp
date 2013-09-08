#define BOOST_TEST_MODULE node_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

struct BeforeTest {
    BeforeTest() : init(5) { }
    global::Sp2pInitializer init;
};

BOOST_FIXTURE_TEST_SUITE(basic_node_test, BeforeTest)

    BOOST_AUTO_TEST_CASE(create_test) {

        types::NodeDescription ndesc;
        ConnectionManager<NodeRequest, NodeResponse> con_m;
        Node node(ndesc, con_m);
    }


BOOST_AUTO_TEST_SUITE_END ( )

