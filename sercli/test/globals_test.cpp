#define BOOST_TEST_MODULE global_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

struct BeforeTest {
    
    BeforeTest() {
        BOOST_TEST_MESSAGE("INIT Global Test");
        global::init(10);
    }

    ~BeforeTest() {
        BOOST_TEST_MESSAGE("Destroy Global Test");
        global::destroyAll();
    }

};

BOOST_FIXTURE_TEST_SUITE(global_test, BeforeTest)

    BOOST_AUTO_TEST_CASE(simple_global) {
        BOOST_CHECK(global::io_s != nullptr);
    }


BOOST_AUTO_TEST_SUITE_END ( )

