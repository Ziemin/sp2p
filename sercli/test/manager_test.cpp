#define BOOST_TEST_MODULE manager_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

BOOST_AUTO_TEST_SUITE(manager_test)

    BOOST_AUTO_TEST_CASE(simple_manager) {
        Manager manager;
    }


BOOST_AUTO_TEST_SUITE_END ( )

