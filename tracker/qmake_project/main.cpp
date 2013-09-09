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
#include "db/dbconfig.h"
#include "constants.h"

using namespace sp2p::tracker;


int main(int argc, char* argv[])
{

    try
    {
        // Check command line arguments.
        DBConnector_ptr DBConnector_ = DBConnector_ptr(new db::PsqlConnector(
                                                           db::config::DATABASE,
                                                           db::config::USER,
                                                           db::config::PASSWORD,
                                                           db::config::ADDRESS,
                                                           db::config::PORT));
        auto sessionControler = SessionControler_ptr(new SessionControler());
        auto factory = std::shared_ptr<protocol_factory::AbstractProtocolFactory>(
                    new ProtocolFactory(sessionControler, DBConnector_));

        Server *server = new Server(factory, consts::SERVER_ADDRESS, consts::SERVER_PORT, consts::THREAT_NUMBER);
        server->run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
