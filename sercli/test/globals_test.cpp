#define BOOST_TEST_MODULE global_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"
#include <boost/log/core.hpp>

using namespace sp2p::sercli;

struct BeforeTest {
    BeforeTest() : init(5) { }
    global::Sp2pInitializer init;
};

BOOST_FIXTURE_TEST_SUITE(global_test, BeforeTest)

    BOOST_AUTO_TEST_CASE(simple_global) {
        BOOST_CHECK(global::io_s != nullptr);
    }


BOOST_AUTO_TEST_SUITE_END ( )

