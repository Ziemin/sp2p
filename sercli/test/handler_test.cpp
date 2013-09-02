#define BOOST_TEST_MODULE handler_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

BOOST_AUTO_TEST_SUITE(handler_test)

    BOOST_AUTO_TEST_CASE(simple_handler_test) {
        Sp2pHandler handler;
    }

BOOST_AUTO_TEST_SUITE_END ( )

