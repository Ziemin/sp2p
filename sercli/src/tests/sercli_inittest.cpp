#define BOOST_TEST_MODULE sercli_inittest
#include <boost/test/unit_test.hpp>
#include <botan/botan.h>
#include <botan/dsa.h>
#include <botan/rsa.h>
#include <botan/x509self.h>

#include <iostream>
#include <fstream>
#include <memory>


int funkcja(int a, int b) { return a+b; }

BOOST_AUTO_TEST_SUITE(sercli_inittest)

BOOST_AUTO_TEST_CASE(test1) {
    int a = 7;
    BOOST_CHECK(a == 7);
}

BOOST_AUTO_TEST_SUITE_END ( )
