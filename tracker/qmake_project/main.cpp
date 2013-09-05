#include <iostream>
#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "types.hpp"
#include "server.hpp"
#include "protocolfactory.hpp"
#include "protocol_factory/abstractprotocolfactory.hpp"
#include "logger.hpp"
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

using namespace sp2p::tracker;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

int main(int argc, char* argv[])
{
//    sp2p::tracker::logger::init_logging();

//    boost::log::sources::severity_channel_logger< sp2p::tracker::logger::severity_level > lg(boost::log::keywords::channel = "net");
//    BOOST_LOG_SEV(lg, sp2p::tracker::logger::warning) << "Hello world!";

    try
    {
        // Check command line arguments.

        auto factory = std::shared_ptr<protocol_factory::AbstractProtocolFactory>(new ProtocolFactory());
        Server *server = new Server(factory, "127.0.0.1", "1234", 1);
        server->run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
