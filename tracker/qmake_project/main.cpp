#include <iostream>
#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "types.hpp"
#include "server.hpp"
#include "logger.hpp"
#include "protocolfactory.hpp"
#include "sessioncontroler.hpp"
#include "db/psqlconnector.h"

using namespace sp2p::tracker;


int main(int argc, char* argv[])
{

    try
    {
        // Check command line arguments.
        auto sessionControler = SessionControler_ptr(new SessionControler());
        auto factory = std::shared_ptr<protocol_factory::AbstractProtocolFactory>(new ProtocolFactory(sessionControler));

        Server *server = new Server(factory, "127.0.0.1", "1234", 1);
        server->run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
