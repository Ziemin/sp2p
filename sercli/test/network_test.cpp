#define BOOST_TEST_MODULE network_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

BOOST_AUTO_TEST_SUITE(network_test)

    BOOST_AUTO_TEST_CASE(simple_network_test) {
        types::NetworkDescription net_desc;
        Network net(net_desc);
    }


BOOST_AUTO_TEST_SUITE_END ( )

