#include <botan/botan.h>
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
using namespace Botan;
using namespace std;

int main() {
	AutoSeeded_RNG rng;
	SymmetricKey key(rng, 32);
	InitializationVector iv(rng, 16);

	Pipe encryptor(get_cipher("AES-256/CBC", key, iv, ENCRYPTION),
			new Base64_Encoder);

	encryptor.start_msg();
	encryptor.write("blablabalba ala ama  duże cycki\n");
	encryptor.end_msg(); // flush buffers, complete computations
	std::cout << encryptor;

	boost::asio::io_service io_s;
}
