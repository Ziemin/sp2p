#define BOOST_TEST_MODULE sercli_inittest
#include <boost/test/unit_test.hpp>
#include <botan/botan.h>
#include <botan/dsa.h>
#include <botan/rsa.h>
#include <botan/x509self.h>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <memory>

BOOST_AUTO_TEST_SUITE(sercli_edu_tests)

BOOST_AUTO_TEST_CASE(private_key) {
	Botan::LibraryInitializer init;

	try {
		Botan::AutoSeeded_RNG rng;
		Botan::RSA_PrivateKey key(rng, 2048);
		std::ofstream priv_key("priv.pem");
		priv_key << Botan::PKCS8::PEM_encode(key, rng, "password");

		Botan::X509_Cert_Options opts;
		opts.common_name = "Some name";

	} catch(std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(rsa_key) {

	Botan::LibraryInitializer init;
	try {
		Botan::AutoSeeded_RNG rng;
		std::ofstream pub_os("rsa_pub");
		std::ofstream priv_os("rsa_priv");
		
		Botan::RSA_PrivateKey private_key(rng, 2048);
		pub_os << Botan::X509::PEM_encode(private_key);
		priv_os << Botan::PKCS8::PEM_encode(private_key);
	} catch(std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(self_signed_cert) {
    Botan::LibraryInitializer init;

	try {
		Botan::AutoSeeded_RNG rng;
		Botan::Private_Key* priv_key = Botan::PKCS8::load_key("rsa_priv", rng);
		BOOST_CHECK(priv_key->algo_name() == "RSA");
		Botan::X509_Cert_Options opts;
		opts.country = "Pl";
		opts.email ="mail@mail.com";
		opts.common_name = "some_shit";
		opts.organization = "home";
		opts.dns = "bla";
		opts.locality = "Europe";

		opts.CA_key();

		Botan::X509_Certificate cert = Botan::X509::create_self_signed_cert(opts, *priv_key, "SHA-256", rng);

		std::ofstream cert_file("cert.pem");
        cert_file << cert.PEM_encode();                        	


		delete priv_key;        
	} catch(std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}
BOOST_AUTO_TEST_CASE(buffers) {

	boost::asio::streambuf sb;
	std::cout << "buffer size: " << sb.size() << std::endl;
	std::ostream os(&sb);
	os << 65;
	std::cout << "buffer size: " << sb.size() << std::endl;
	std::istream is(&sb);
	int number;
	is >> number;
    BOOST_TEST_CHECKPOINT("sth");
    BOOST_TEST_MESSAGE("other sth");
	std::cout << "buffer size: " << sb.size() << std::endl;
	std::cout << "number: " << number << std::endl;
	os << "alal ma kota";
	std::cout << "buffer size: " << sb.size() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END ( )
