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

        Server *server = new Server(factory, "127.0.0.1", "1234", 1);


        using namespace std;
        string a = "a", b = "b", c = "c", d = "d",\
                alice = "Alice",
                bob = "Bob",
                charlie = "Charlie",
                deryl = "Deryl",
                ftp = "ftp",
                http = "http",
                onet = "onet",
                interia = "interia",
                pass = "123",
                mail = "asd@qwe.pl";
//        DBConnector_->createInvitation(bob, http);
//        std::cout << "Alice:\n";
        //        DBConnector_->getAllNetworks(alice);
        //        std::cout << "Bob:\n";
        //        DBConnector_->getAllNetworks(bob);
        //        std::cout << "Charlie:\n";
        //        DBConnector_->getAllNetworks(charlie);
        //        std::cout << "Deryl:\n";
        //        DBConnector_->getAllNetworks(deryl);
        //        DBConnector_->deleteNetwork(alice, ftp);
        DBConnector_->deleteNetwork(alice, http);
        DBConnector_->deleteNetwork(deryl, http);

        //        server->run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
