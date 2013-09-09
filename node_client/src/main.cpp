#include "../../sercli/src/sp2p.hpp"

#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/log/core.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "context.hpp"

using namespace std;
namespace po = boost::program_options;
namespace sc = sp2p::sercli;

const char* ABOUT = "NodeClient is a simple program presenting usage of sp2p library.\n"
                    "It allows registering users, networks and managing related to sp2p data.\n"
                    "To get familiar with program usage type help during runtime\n";

const unsigned int ARGS_SIZE = 100;

namespace config {
    int running_threads;
    string dataFile;
    string password;
}

vector<string> split_input(const string& input) {
    stringstream ss(input);
    vector<string> splitted;

    std::copy(istream_iterator<string>(ss), istream_iterator<string>(), std::back_inserter(splitted));
    return splitted;
}

inline void place_arguments(const char* arguments[], const vector<string>& input) {
    for(size_t i = 0; i < input.size(); i++) {
        if(i == ARGS_SIZE) {
            cout << "Too many arguments";
            return;
        }
        arguments[i] = input[i].c_str();
    }
}

void run_program_loop(RootContext& dispatcher) {
    string command_line;
    const char* arguments[ARGS_SIZE];
    std::vector<string> splitted;
    while(true) {
        try {
            cout << "> ";
            if(!getline(cin, command_line)) throw ExitException();
            splitted = split_input(command_line);
            if(splitted.empty()) continue;
            place_arguments(arguments, splitted);

            dispatcher[splitted[0]](splitted.size(), arguments);

        } catch(sc::NodeException& ne) {
            cerr << "Node exception: " << ne.what() << endl;
        } catch(sc::NetworkException nete) {
            cerr << "Network exception: " << nete.what() << endl;
        } catch(sc::types::NodeError& error) {
            cerr << "Node error: " << error << endl;
        } catch(po::error& pe) {
            cerr << "Program option exception: " << pe.what() << endl;
        } catch(CommandException& ce) {
            cerr << "Command exception: " << ce.what() << endl;
        } catch(ExitException& ee) {
            cout << "Exiting" << endl;
            boost::log::core::get()->flush();
            return;
        }
    }
}

/**
 * Simple program to communicate with node and manage 
 * sp2p related data.
 * It primarily makes use of sp2p sercli library.
 */
int main(int argc, char *argv[]) {

    try {

        po::options_description odesc("Program options");
        odesc.add_options()
            ("help,h",                                                                      "print this message")
            ("about,a",                                                                     "about NodeClient")
            ("include-file,I",  po::value<string>(&config::dataFile),                       "file with saved data")
            ("password,P",      po::value<string>(&config::password),                       "password to decode the file")
            ("threads,T",       po::value<int>(&config::running_threads)->default_value(5), "threads in pool");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, odesc), vm);
        po::notify(vm);


        if(vm.count("help")) {
            cout << odesc << endl;
            return 0;
        } else if(vm.count("about")) {
            cout << ABOUT << endl;
            return 0;
        } 

        if(!vm.count("include-file")) {
            cerr << "no include-file given" << endl;
            return 1;
        } else if(!vm.count("password")) {
            cerr << "no password given" << endl;
            return 1;
        }

        sc::global::Sp2pInitializer initer(config::running_threads);

        RootContext dispatcher("MainApp");
        Sp2pContext sp2p_context(&dispatcher, config::dataFile, config::password);

        dispatcher+(&sp2p_context);

        run_program_loop(dispatcher);

    } catch(exception& e) {
        cerr << "exception: " << e.what() << endl;
    } catch(...) {
        cerr << "exception of unknown type" << endl;
    }

    return 0;
}
