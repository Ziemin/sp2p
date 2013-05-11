#define BOOST_TEST_MODULE sercli_inittest
#include <boost/test/unit_test.hpp>

int funkcja(int a, int b) { return a+b; }

BOOST_AUTO_TEST_SUITE(sercli_inittest)

BOOST_AUTO_TEST_CASE(test1) {
    int a = 7;
    BOOST_CHECK(a == 7);
}

BOOST_AUTO_TEST_SUITE_END ( )
