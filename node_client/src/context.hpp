#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "../../sercli/src/sp2p.hpp"
#include <boost/program_options.hpp>
#include <boost/log/core.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <exception>
#include <map>
#include <functional>
#include <boost/asio.hpp>
#include <tuple>
#include <signal.h>
#include <boost/regex.hpp>
#include <botan/botan.h>
#include <botan/x509cert.h>
#include <botan/pem.h>
#include <botan/rsa.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;
namespace sc = sp2p::sercli;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

void disableEnableEcho(bool enable = true) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if(!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

const string NODE_CONTEXT_NAME = "Node";
const string NETWORK_CONTEXT_NAME = "Network";

void getFilenames(const string& dir, vector<string>& filenames) {
    fs::path pt = fs::system_complete(fs::path(dir));
    if(!fs::exists(pt)) throw "Path does no exists";
    if(!fs::is_directory(pt)) throw "Path is not a directory";

    string filename;
    fs::directory_iterator end_it;
    for(fs::directory_iterator it(pt); it != end_it; ++it) {
        if(!fs::is_directory(it->path())) {
            filenames.push_back(filename = it->path().filename().string());
        }
    }
}

class CommandException : exception {

    public:
        CommandException(string message) : message(move(message)) { }

        const char* what() const throw() {
            return message.c_str();
        }

    private:
        string message;
};

class ExitException : public exception {

};

typedef function<void(int, const char *argv[])> Handler ;

class Context {

    friend ostream& operator<<(ostream& os, const Context& context);

    public: // Constructors
        Context(string context_name, Context* root=nullptr) 
            : context_name(move(context_name)), 
            current_context(this),
            root(root)
        {

                if(root == nullptr) root = this;

                handlers["current"] = [this](int argc, const char *argv[]) -> void {
                    cout << "Current context: " << ((Context&)*(this->root->current_context)) << endl;
                };

                handlers["pr"] = [this](int argc, const char *argv[]) -> void {
                    if(parent_context != nullptr) {
                        this->root->current_context=parent_context;
                    }
                    else throw CommandException("No parent exists for current context");
                }; 

                handlers["cd"] = [this](int argc, const char *argv[]) -> void {
                    if(argc <= 1) 
                        throw CommandException("No child name given");

                    string child(argv[1]);
                    auto it = nested_contexts.find(child);
                    if(it == nested_contexts.end()) 
                        throw CommandException("No child with given name: " + child);

                    this->root->current_context = it->second;
                }; 

                handlers["list"] = [this](int argc, const char* argv[]) -> void {
                    cout << "Child contexts:" << endl;
                    for(auto context_pair: nested_contexts) {
                        cout << "- " << *context_pair.second << endl;
                    }
                    cout << "Parent context: " << endl 
                        << "- " + (parent_context == nullptr ? "none" : parent_context->context_name) << endl;
                };

                handlers["exit"] = [](int argc, const char *argv[]) -> void {
                    throw ExitException();
                };

                handlers["print"] = [this](int argc, const char *argv[]) -> void {
                    cout << "Available commands:" << endl;
                    for(auto handler_pair: handlers) {
                        cout << "- " << handler_pair.first << endl;
                    }
                };

                handlers["help"] = [this](int, const char *argv[]) -> void {
                    help_message();
                };

                // flushing logs
                handlers["flush"] = [this](int, const char *argv[]) -> void {
                    boost::log::core::get()->flush();
                };
        } // ~ Constructors

    public:
        virtual ~Context() = default;

        Handler& operator[](const string& command) {
            auto it = handlers.find(command);
            if(it == handlers.end()) 
                throw CommandException("No such command avaialable in current context");

            return handlers[command];
        }

        Context& operator+(Context* child) {
            nested_contexts[child->context_name] = child;
            child->parent_context = this;
            child->root = this->root;
            return *child;
        }

        Context& operator-() {
            return *parent_context;
        }

    protected:

        virtual void help_message() const {
            cout << "Basic commands: " << endl;
            cout << "current"       << "\t\t - Prints current context" << endl;
            cout << "pr"            << "\t\t - Switches to parent context" << endl;
            cout << "cd (name)"     << "\t - Sets current context to child context" << endl;
            cout << "list"          << "\t\t - Prints all children and parent contexts" << endl;
            cout << "exit"          << "\t\t - Exits application" << endl;
            cout << "print"         << "\t\t - Prints all avaialable commands within current context" << endl;
            cout << "flush"         << "\t\t - Flushes logs to file" << endl;
        }

    public:
        const string context_name;
        Context* current_context;

    protected:
        map<string, Context*> nested_contexts;
        Context* parent_context = nullptr;
        Context* root;
        map<string, Handler> handlers;

}; 

ostream& operator<<(ostream& os, const Context& context) {
    os << context.context_name;
    return os;
}

/* ------------------ ~ Context ------------------------------------------------- */

class RootContext : public Context {

    public:
        RootContext(const string& name) : Context(name, this) { }

        Handler& operator[](const string& command) {
            if(current_context == this) {
                return Context::operator[](command);
            }
            return (*current_context)[command];
        }
};

/* ------------------ ~ RootContext ------------------------------------------------- */


class NodeContext : public Context {

    friend class Sp2pContext;

    public:

        NodeContext(Context* root, sc::Manager& manager) : Context(NODE_CONTEXT_NAME, root), sp2p_manager(manager) {

            // print current node
            handlers["pnd"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                cout << node->getDescription() << endl;
                cout << node->getUser() << endl;
            };

            // print current network
            handlers["pnt"] = [this](int argc, const char *argv[]) -> void {
                checkNetwork();
                cout << network->getDescription() << endl;
            };

            // set user
            handlers["setU"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                vm.clear();
                po::store(po::parse_command_line(argc, argv, su_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << su_opts << endl;
                    return;
                }

                sc::MyUser new_user;
                if(vm.count("username")) {
                    new_user.username = vm["username"].as<string>();
                } else throw CommandException("username required");
                if(vm.count("password")) {
                    new_user.password = vm["password"].as<string>();
                } else throw CommandException("password required");
                if(vm.count("registered")) new_user.is_registered = vm["registered"].as<bool>();

                if(vm.count("email")) 
                    new_user.email = vm["email"].as<string>();
                else 
                    new_user.email = "";

                node->setUser(new_user);
            };

            // login
            handlers["login"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                sc::types::NodeError res = node->logIn();
                cout << "Resp: " << res << endl;
            };

            // logout
            handlers["logout"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                sc::types::NodeError res = node->logOut();
                cout << "Resp: " << res << endl;
            };

            // register new user
            handlers["register"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                sc::types::NodeError res = node->registerUser();
                cout << "Resp: " << res << endl;
            };

            // change password
            handlers["cp"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                vm.clear();
                po::store(po::parse_command_line(argc, argv, cp_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << cp_opts << endl;
                    return;
                }

                if(vm.count("password")) {
                    sc::types::NodeError res = node->changePassword(vm["password"].as<string>());
                    if(sc::types::any(res)) throw res;
                } else throw CommandException("Password required");
            };

            // print user
            handlers["user"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                cout << node->getUser() << endl;
            };

            // get user
            handlers["getU"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                vm.clear();
                po::store(po::parse_command_line(argc, argv, getU_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << cp_opts << endl;
                }

                if(vm.count("username")) {
                    tuple<sc::types::NodeError, sc::User> res = 
                        node->getUserInfo(network->getDescription(), vm["username"].as<string>());
                    if(any(get<0>(res))) cout << "Error: " << get<0>(res) << endl;
                    else cout << get<1>(res) << endl;
                } else throw CommandException("Username required");
            };

            // Invite user
            handlers["invite"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();

                vm.clear();
                po::store(po::parse_command_line(argc, argv, invite_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << cp_opts << endl;
                }

                if(vm.count("username")) {
                    sc::types::NodeError res= node->inviteUser(network->getDescription(), { vm["username"].as<string>() });
                    cout << "Resp: " << res << endl;
                } else throw CommandException("Username required");
            };


            // get networks list
            handlers["getNL"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                tuple<sc::types::NodeError, vector<sc::types::NetworkDescription>> res =
                    node->getNetworksList();
                if(any(get<0>(res))) cout << "Error: " << get<0>(res) << endl;
                else {
                    for(sc::types::NetworkDescription& net_desc: get<1>(res)) 
                        cout << net_desc << endl;
                }
            };

            // get my networks
            handlers["getMN"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                tuple<sc::types::NodeError, vector<sc::types::NetworkDescription>> res =
                    node->getMyNetworks();
                if(any(get<0>(res))) cout << "Error: " << get<0>(res) << endl;
                else {
                    for(sc::types::NetworkDescription& net_desc: get<1>(res)) 
                        cout << net_desc << endl;
                }
            };

            // get server list
            handlers["getSL"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                tuple<sc::types::NodeError, vector<sc::types::ServerDescription>> res =
                    node->getServersList(network->getDescription());
                if(any(get<0>(res))) cout << "Error: " << get<0>(res) << endl;
                else {
                    for(sc::types::ServerDescription& serv_desc: get<1>(res)) 
                        cout << serv_desc << endl;
                }
            };

            // is active?
            handlers["ia"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                if(node->isActive()) cout << "Yes" << endl;
                else cout << "No" << endl;
            };

            // is active?
            handlers["il"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                if(node->isLogged()) cout << "Yes" << endl;
                else cout << "No" << endl;
            };

            // is registered?
            handlers["ir"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                if(node->isRegistered()) cout << "Yes" << endl;
                else cout << "No" << endl;
            };

            // register network
            handlers["regNet"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                sc::types::NodeError resp = node->registerNetwork(network->getDescription());
                cout << resp << endl;
            };

            // delete network
            handlers["delNet"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                sc::types::NodeError resp = node->deleteNetwork(network->getDescription());
                cout << resp << endl;
            };

            // update server
            handlers["ups"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                vm.clear();
                po::store(po::parse_command_line(argc, argv, ups_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << ups_opts << endl;
                    return;
                }

                if(vm.count("port")) {
                    tuple<sc::types::NodeError, uint16_t> res = 
                        node->updateServer(network->getDescription(), vm["port"].as<uint16_t>());
                    if(any(get<0>(res))) {
                        cout << "Error: " << get<0>(res) << endl;
                    } else {
                        cout << "Time to next update: " << get<1>(res) << endl;
                    }
                } else throw CommandException("Port required");
            };

            // stop server
            handlers["ss"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();

                sc::types::NodeError resp = node->stopServer(network->getDescription());
                cout << resp << endl;
            };

            // stop connections
            handlers["stop"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                node->stopConnections();
            };

            // help
            handlers["help"] = [this](int argc, const char *argv[]) -> void {
                help_message();
            };

            // generate private key
            handlers["gk"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                vm.clear();
                po::store(po::parse_command_line(argc, argv, gk_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << gk_opts << endl;
                    return;
                }

                string password;
                if(vm.count("password")) password = vm["password"].as<string>();
                else throw CommandException("No password given");

                sc::enc::priv_st_ptr priv_key(new sc::enc::PrivateKeyStore(sc::enc::generatePrivateKey(2048)));
                priv_key->setPassword(password);

                if(vm.count("network")) {
                    sc::network_ptr net = sp2p_manager.getNetwork(vm["network"].as<string>());
                    priv_key->setName(node->getDescription().node_name + "." + net->getDescription().network_name);
                    priv_key->setFilename(priv_key->getName() + ".pem");
                    priv_key->setPath(priv_key_dir);
                    node->getNetworkKeys()[net->getDescription()] = priv_key;
                } else {
                    priv_key->setName(node->getDescription().node_name);
                    priv_key->setFilename(priv_key->getName() + ".pem");
                    priv_key->setPath(priv_key_dir);
                    node->getMyKeys().push_back(priv_key);
                }
                priv_key->save();
            };

            // sign key
            handlers["sk"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                vm.clear();
                po::store(po::parse_command_line(argc, argv, sk_opts), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << sk_opts << endl;
                    return;
                }
                if(vm.count("network")) {
                    sc::network_ptr net = sp2p_manager.getNetwork(vm["network"].as<string>());
                    sc::enc::priv_st_ptr key = node->getNetworkKeys()[net->getDescription()];
                    Botan::Private_Key* priv_key = key->getKey();
                    tuple<NodeError, Botan::X509_Certificate*> res = node->signKey(*priv_key, &net->getDescription());
                    cout << get<0>(res) << endl;
                    if(!sc::types::any(get<0>(res))) {
                        Botan::X509_Certificate* cert = get<1>(res);
                        cout << "Cert: " << endl <<  cert->to_string() << endl;
                    }
                } else throw CommandException("Network name required");
            };
        }

        void setPrivKeyDir(const std::string& dir) {
            priv_key_dir = dir;
        }
        void setCertDir(const std::string& dir) {
            cert_dir = dir;
        }

    private:

        void setNode(sc::node_ptr node) { this->node = node; }
        void setNetwork(sc::network_ptr network) { this->network = network; }

        void checkNode() {
            if(node == nullptr) throw sc::NodeException("No working node set");
        }

        void checkNetwork() {
            if(network == nullptr) throw sc::NetworkException("No working network set");
        }

        void help_message() const override {
            Context::help_message();
            cout << "Node commands: " << endl;
            cout << "setU"      << "\t\t - Sets new user for node" << endl;
            cout << "login"     << "\t\t - Logs to node" << endl;
            cout << "logout"    << "\t\t - Logs out" << endl;
            cout << "register"  << "\t - Registers new user" << endl;
            cout << "cp"        << "\t\t - Changes password" << endl;
            cout << "user"      << "\t\t - Prints current user" << endl;
            cout << "getU"      << "\t\t - Gets user and prints info" << endl;
            cout << "getNL"     << "\t\t - Gets networks list" << endl;
            cout << "getMN"     << "\t\t - Gets my networks list" << endl;
            cout << "getSL"     << "\t\t - Gets visible servers" << endl;
            cout << "ia"        << "\t\t - Is active?" << endl;
            cout << "ir"        << "\t\t - Is user registered?" << endl;
            cout << "il"        << "\t\t - Is user logged?" << endl;
            cout << "regNet"    << "\t\t - Registers network" << endl;
            cout << "delNet"    << "\t\t - Deletes network" << endl;
            cout << "ups"       << "\t\t - Updates server" << endl;
            cout << "ss"        << "\t\t - Stops server" << endl;
            cout << "stop"      << "\t\t - Stops connection with node" << endl;
            cout << "invite"    << "\t\t - Invite user to network" << endl;
            cout << "gk"        << "\t\t - Generate private key" << endl;
            cout << "sk"        << "\t\t - Sign key" << endl;
        }

    private:
        sc::node_ptr node = nullptr;
        sc::network_ptr network = nullptr;
        sc::Manager& sp2p_manager;
        po::variables_map vm;
        string priv_key_dir;
        string cert_dir;

        static po::options_description su_opts;
        static po::options_description cp_opts;
        static po::options_description getU_opts;
        static po::options_description ups_opts;
        static po::options_description invite_opts;
        static po::options_description gk_opts;
        static po::options_description sk_opts;

};

po::options_description NodeContext::su_opts = []() {

    po::options_description opts("Set user");
    opts.add_options()
        ("help,h", "prints this message")
        ("username,u", po::value<string>(), "username")
        ("email,e", po::value<string>(), "email")
        ("password,p", po::value<string>(), "password")
        ("registered,r", po::value<bool>()->default_value(false), "is registered (0 or 1)");

    return opts;
}();

po::options_description NodeContext::cp_opts = []() {

    po::options_description opts("Change password");
    opts.add_options()
        ("help,h", "prints this message")
        ("password,p", po::value<string>(), "password");

    return opts;
}();

po::options_description NodeContext::getU_opts = []() {

    po::options_description opts("Get info about user");
    opts.add_options()
        ("help,h", "prints this message")
        ("username,u", po::value<string>(), "username");

    return opts;
}();

po::options_description NodeContext::invite_opts = []() {

    po::options_description opts("Invite user");
    opts.add_options()
        ("help,h", "prints this message")
        ("username,u", po::value<string>(), "username");

    return opts;
}();

po::options_description NodeContext::ups_opts = []() {

    po::options_description opts("Update server");
    opts.add_options()
        ("help,h", "prints this message")
        ("port,p", po::value<uint16_t>(), "port");

    return opts;
}();

po::options_description NodeContext::gk_opts = []() {

    po::options_description opts("Generate private key");
    opts.add_options()
        ("help,h", "prints this message")
        ("network,n", po::value<string>(), "Optional - generate key for network")
        ("password,p", po::value<string>(), "Password for key");

    return opts;
}();

po::options_description NodeContext::sk_opts = []() {

    po::options_description opts("Sign key");
    opts.add_options()
        ("help,h", "prints this message")
        ("network,n", po::value<string>(), "Optional - generate key for network");

    return opts;
}();

/* ------------------ ~ NodeContext ------------------------------------------------- */

class NetworkContext : public Context {

    friend class Sp2pContext;
    
    public:
        NetworkContext(Context* root) : Context(NETWORK_CONTEXT_NAME, root) {

            // print current network
            handlers["pnt"] = [this](int argc, const char *argv[]) -> void {
                checkNetwork();
                cout << network->getDescription() << endl;
            };

            // print current node
            handlers["pnd"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                cout << node->getDescription() << endl;
                cout << node->getUser() << endl;
            };

            // associate node
            handlers["addNd"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                network->associateNode(node);
            };

            // detach node
            handlers["delNd"] = [this](int argc, const char *argv[]) -> void {
                checkNode();
                checkNetwork();
                network->detachNode(node->getDescription());
            };
            
            // get associated nodes
            handlers["getNds"] = [this](int argc, const char *argv[]) -> void {
                checkNetwork();
                for(auto& node_pair: network->getAssociatedNodes()) 
                    cout << node_pair.first << endl; 
            };

            // get available servers
            handlers["getSl"] = [this](int argc, const char *argv[]) -> void {
                checkNetwork();
                for(sc::types::ServerDescription& serv_desc: network->getAvailableServers())  
                    cout << serv_desc << endl;
            };

            // is active?
            handlers["ia"] = [this](int argc, const char *argv[]) -> void {
                checkNetwork();
                if(network->isActive()) cout << "Yes" << endl;
                else cout << "No" << endl;
            };
        }

        void setNetwork(sc::network_ptr network) { this->network = network; }
        void setNode(sc::node_ptr node) { this->node = node; }

    private:
        void checkNode() {
            if(node == nullptr) throw sc::NodeException("No working node set");
        }

        void checkNetwork() {
            if(network == nullptr) throw sc::NetworkException("No working network set");
        }

        void help_message() const override {
            Context::help_message();
            cout << "Network commands: " << endl;
            cout << "pnt"       << "\t\t - Print current network" << endl;
            cout << "pnd"       << "\t\t - Print current node" << endl;
            cout << "addNd"     << "\t\t - Associates node with network" << endl;
            cout << "delNd"     << "\t\t - Detaches node from network" << endl;
            cout << "getNds"    << "\t\t - Gets all related nodes" << endl;
            cout << "getSl"     << "\t\t - Gets all servers from network" << endl;
            cout << "ia"        << "\t\t - Is active?" << endl;
        }

    private:
        sc::network_ptr network = nullptr;
        sc::node_ptr node = nullptr;
};

/* ------------------ ~ NetworkContext ------------------------------------------------- */

class Sp2pContext : public Context {

    public:

        Sp2pContext(Context* root, const string& dataFile, const string& password, const string& config_file)
        : Context("Sp2p", root),  
        data_manager(password),
        sp2p_manager(data_manager),
        node_c(root, sp2p_manager),
        network_c(root),
        signals(*sc::global::io_s)
        {

            //signals.add(SIGINT);
            //signals.add(SIGTERM);
            //signals.add(SIGQUIT);
            await_stop();

            data_manager.setDataFile(dataFile);
            ifstream ifs(dataFile);
            if(ifs) sp2p_manager.loadState();

            (*this)+(&node_c);
            (*this)+(&network_c);

            my_user.add(user);

            create_node.add(node_desc).add(my_user);
            get_node.add(node_desc);
            remove_node.add(node_desc);

            create_network.add(network_desc);
            get_network.add(network_desc);
            remove_network.add(network_desc);

            ifstream iconf(config_file);
            if(!iconf) throw "Could not open config file";

            po::store(po::parse_config_file(iconf, load_conf_opts), vm);
            po::notify(vm);
            
            if(vm.count("node_name") && vm["node_name"].as<string>().size() > 0) {
                sc::node_ptr node = sp2p_manager.getNode(vm["node_name"].as<string>());
                node_c.setNode(node);
                network_c.setNode(node);
                current_node = node;
            }
            if(vm.count("network_name") && vm["network_name"].as<string>().size() > 0) {
                sc::network_ptr network = sp2p_manager.getNetwork(vm["network_name"].as<string>());
                node_c.setNetwork(network);
                network_c.setNetwork(network);
                current_network = network;
            }
            if(vm.count("priv_keys_dir")) {
                scanForPrivKeys(vm["priv_keys_dir"].as<string>());
                node_c.setPrivKeyDir(vm["priv_keys_dir"].as<string>());
                priv_keys_dir = vm["priv_keys_dir"].as<string>();
            } else throw "No private keys directory specified";

            if(vm.count("pub_keys_dir")) {
                scanForPubKeys(vm["pub_keys_dir"].as<string>());
                pub_keys_dir = vm["pub_keys_dir"].as<string>();
            } else throw "No public keys directory specified";

            if(vm.count("certs_dir")) {
                scanForCerts(vm["certs_dir"].as<string>());
                node_c.setCertDir(vm["certs_dir"].as<string>());
                cert_dir = vm["certs_dir"].as<string>();
            } else throw "No certificates directory specified";

            // initialization of handlers
            // create node
            handlers["cnd"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, create_node), vm);
                po::notify(vm);

                if(vm.count("help")) {
                  cout << create_node << endl;
                  return;
                }
                sc::types::NodeDescription description;
                if(vm.count("name")) {
                    description.node_name  = vm["name"].as<string>();
                } else throw CommandException("Node name required");
                if(vm.count("port")) {
                    description.port = vm["port"].as<uint16_t>();
                } else throw CommandException("Port required");
                if(vm.count("ip-address")) {
                    description.ip_address = 
                        boost::asio::ip::address::from_string(vm["ip-address"].as<string>());
                } else throw CommandException("IP address requiered");
                
                sc::MyUser new_user;
                if(vm.count("username")) {
                    new_user.username = vm["username"].as<string>();
                    if(vm.count("password")) 
                        new_user.password = vm["password"].as<string>();
                    else 
                        throw CommandException("User requires password");
                    if(vm.count("registered")) 
                        new_user.is_registered = vm["registered"].as<bool>();
                    if(vm.count("email")) 
                        new_user.email = vm["email"].as<string>();
                    else 
                        new_user.email = "";

                    sp2p_manager.createNode(description, new_user);

                } else {
                    sp2p_manager.createNode(description);
                }
            };

            // get node / goto node
            handlers["gnd"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, get_node), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << get_node << endl;
                    return;
                }

                sc::types::NodeDescription description;
                if(vm.count("name")) {
                    description.node_name = vm["name"].as<string>();
                } else throw CommandException("Node name required");
                int params = 1;
                if(vm.count("port")) {
                    description.port = vm["port"].as<uint16_t>();
                    params++;
                }
                if(vm.count("ip-address")) {
                    description.ip_address =
                        boost::asio::ip::address::from_string(vm["ip-address"].as<string>());
                    params++;
                }
                sc::node_ptr node;
                if(params == 3) node = sp2p_manager.getNode(description);
                else node = sp2p_manager.getNode(description.node_name);

                node_c.setNode(node);
                network_c.setNode(node);
                current_node = node;
            };

            // remove node
            handlers["rnd"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, remove_node), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << remove_node << endl;
                    return;
                }

                sc::types::NodeDescription description;
                if(vm.count("name")) {
                    description.node_name = vm["name"].as<string>();
                } else throw CommandException("Node name required");
                int params = 1;
                if(vm.count("port")) {
                    description.port = vm["port"].as<uint16_t>();
                    params++;
                }
                if(vm.count("ip-address")) {
                    description.ip_address =
                        boost::asio::ip::address::from_string(vm["ip-address"].as<string>());
                    params++;
                }
                if(params == 3) sp2p_manager.removeNode(description);
                else sp2p_manager.removeNode(description.node_name);
            };

            // print all nodes
            handlers["pnd"] = [this](int argc, const char *argv[]) -> void {
                for(auto np: sp2p_manager.getAllNodes()) {
                    cout << "\t" << np->getDescription() << endl;
                    cout << "\t" << np->getUser() << endl;
                }
                cout << endl;
            };

            // create network
            handlers["cnt"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, create_network), vm);
                po::notify(vm);

                if(vm.count("help")) {
                  cout << create_network << endl;
                  return;
                }
                sc::types::NetworkDescription description;
                if(vm.count("name")) {

                    description.network_name  = vm["name"].as<string>();
                } else throw CommandException("Network name required");

                if(vm.count("protocol")) {

                    description.protocol_name = vm["protocol"].as<string>();
                } else throw CommandException("Protocol name required");

                if(vm.count("access_rights")) {

                    string ar = vm["access_rights"].as<string>();
                    if(ar == "PUBLIC") description.access_rights = sc::types::NetworkDescription::AccessRights::PUBLIC;
                    else if(ar == "PRIVATE") description.access_rights= sc::types::NetworkDescription::AccessRights::PRIVATE;
                    else throw CommandException("No such access rights");
                } else throw CommandException("Access Rights required");

                if(vm.count("visibility")) {

                    string ar = vm["visibility"].as<string>();
                    if(ar == "LISTED") description.visability = sc::types::NetworkDescription::Visibility::LISTED;
                    else if(ar == "UNLISTED") description.visability = sc::types::NetworkDescription::Visibility::UNLISTED;
                    else throw CommandException("No such visibility options");
                } else throw CommandException("Visibility required");

                if(vm.count("participation")) {

                    string ar = vm["participation"].as<string>();
                    if(ar == "CLIENT_ONLY") description.participation_rights =
                        sc::types::NetworkDescription::ParticipationRights::CLIENT_ONLY;
                    else if(ar == "CLIENT_SERVER") description.participation_rights=
                        sc::types::NetworkDescription::ParticipationRights::CLIENT_SERVER;
                    else throw CommandException("No such participation rights");
                } else throw CommandException("Participation Rights required");
                
                sp2p_manager.createNetwork(description);
            };

            // get network / goto network
            handlers["gnt"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, get_network), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << get_network << endl;
                    return;
                }

                sc::types::NetworkDescription description;
                if(vm.count("name")) {
                    description.network_name = vm["name"].as<string>();
                } else throw CommandException("Node name required");
                int params = 1;

                if(vm.count("port")) {

                    description.protocol_name = vm["protocol"].as<string>();
                    params++;
                } 
                if(vm.count("access_rights")) {

                    string ar = vm["access_rights"].as<string>();
                    if(ar == "PUBLIC") description.access_rights = sc::types::NetworkDescription::AccessRights::PUBLIC;
                    else if(ar == "PRIVATE") description.access_rights= sc::types::NetworkDescription::AccessRights::PRIVATE;
                    else throw CommandException("No such access rights");
                    params++;
                } 
                if(vm.count("visibility")) {

                    string ar = vm["Visibility"].as<string>();
                    if(ar == "LISTED") description.visability = sc::types::NetworkDescription::Visibility::LISTED;
                    else if(ar == "UNLISTED") description.visability = sc::types::NetworkDescription::Visibility::UNLISTED;
                    else throw CommandException("No such visibility options");
                    params++;
                } 
                if(vm.count("participation")) {

                    string ar = vm["participation"].as<string>();
                    if(ar == "CLIENT_ONLY") description.participation_rights =
                        sc::types::NetworkDescription::ParticipationRights::CLIENT_ONLY;
                    else if(ar == "CLIENT_SERVER") description.participation_rights=
                        sc::types::NetworkDescription::ParticipationRights::CLIENT_SERVER;
                    else throw CommandException("No such participation rights");
                    params++;
                } 

                sc::network_ptr network;
                if(params == 5) network = sp2p_manager.getNetwork(description);
                else network = sp2p_manager.getNetwork(description.network_name);

                network_c.setNetwork(network);
                node_c.setNetwork(network);
                current_network = network;
            };

            // remove network
            handlers["rnt"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, remove_network), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << remove_node << endl;
                    return;
                }

                sc::types::NetworkDescription description;
                if(vm.count("name")) {
                    description.network_name = vm["name"].as<string>();
                } else throw CommandException("Node name required");
                int params = 1;

                if(vm.count("port")) {

                    description.protocol_name = vm["protocol"].as<string>();
                    params++;
                } 
                if(vm.count("access_rights")) {

                    string ar = vm["access_rights"].as<string>();
                    if(ar == "PUBLIC") description.access_rights = sc::types::NetworkDescription::AccessRights::PUBLIC;
                    else if(ar == "PRIVATE") description.access_rights= sc::types::NetworkDescription::AccessRights::PRIVATE;
                    else throw CommandException("No such access rights");
                    params++;
                } 
                if(vm.count("visibility")) {

                    string ar = vm["Visibility"].as<string>();
                    if(ar == "LISTED") description.visability = sc::types::NetworkDescription::Visibility::LISTED;
                    else if(ar == "UNLISTED") description.visability = sc::types::NetworkDescription::Visibility::UNLISTED;
                    else throw CommandException("No such visibility options");
                    params++;
                } 
                if(vm.count("participation")) {

                    string ar = vm["participation"].as<string>();
                    if(ar == "CLIENT_ONLY") description.participation_rights =
                        sc::types::NetworkDescription::ParticipationRights::CLIENT_ONLY;
                    else if(ar == "CLIENT_SERVER") description.participation_rights=
                        sc::types::NetworkDescription::ParticipationRights::CLIENT_SERVER;
                    else throw CommandException("No such participation rights");
                    params++;
                } 

                sc::network_ptr network;
                if(params == 5) sp2p_manager.removeNetwork(description);
                else sp2p_manager.removeNetwork(description.network_name);
            };

            // print all networks
            handlers["pnt"] = [this](int argc, const char *argv[]) -> void {
                for(auto np: sp2p_manager.getAllNetworks()) {
                    cout << "\t" << np->getDescription() << endl;
                }
                cout << endl;
            };

            // save state
            handlers["save"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, save_state), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << save_state << endl;
                    return;
                }

                if(vm.count("filename")) {
                    string filename = vm["filename"].as<string>();
                    string password;
                    if(vm.count("password")) {
                        password = vm["password"].as<string>();
                    } else throw CommandException("Filename requires password to encode");
                    sp2p_manager.saveState(filename);
                } else 
                    sp2p_manager.saveState();

                for(auto node_ptr: sp2p_manager.getAllNodes()) {
                    for(sc::enc::cert_st_ptr cert: node_ptr->getNodeCerts()) {
                        if(!cert->inFile(cert_dir)) cert->save(cert_dir);
                    }
                    for(sc::enc::cert_st_ptr cert: node_ptr->getFreeCerts()) {
                        if(!cert->inFile(cert_dir)) cert->save(cert_dir);
                    }
                    for(auto cert_pair: node_ptr->getNetworkCerts()) {
                        if(!cert_pair.second->inFile(cert_dir)) cert_pair.second->save(cert_dir);
                    }
                    for(sc::enc::priv_st_ptr key: node_ptr->getMyKeys()) {
                        if(!key->inFile(priv_keys_dir)) key->save(priv_keys_dir);
                    }
                    for(auto key_pair: node_ptr->getNetworkKeys()) {
                        if(!key_pair.second->inFile(priv_keys_dir)) key_pair.second->save(priv_keys_dir);
                    }
                }
            };

            // load state
            handlers["load"] = [this](int argc, const char *argv[]) -> void {
                vm.clear();
                po::store(po::parse_command_line(argc, argv, load_state), vm);
                po::notify(vm);

                if(vm.count("help")) {
                    cout << load_state << endl;
                    return;
                }

                if(vm.count("filename")) {
                    string filename = vm["filename"].as<string>();
                    string password;
                    if(vm.count("password")) {
                        password = vm["password"].as<string>();
                    } else throw CommandException("Filename requires password to decode");
                    sp2p_manager.loadState(filename);
                } else 
                    sp2p_manager.loadState();
            };

            handlers["priv_dir"] = [this](int argc, const char *argv[]) -> void {
                cout << " " << priv_keys_dir << endl;
            };
            handlers["pub_dir"] = [this](int argc, const char *argv[]) -> void {
                cout << " " << pub_keys_dir << endl;
            };
            handlers["cert_dir"] = [this](int argc, const char *argv[]) -> void {
                cout << " " << cert_dir << endl;
            };

            // Clear
            handlers["clear"] = [this](int argc, const char *argv[]) -> void {
                sp2p_manager.clear();
            };
        }

     private:
        void help_message() const override {
            Context::help_message();
            cout << "Sp2p commands: " << endl;
            cout << "cnd"           << "\t\t - Add new node" << endl;
            cout << "gnd"           << "\t\t - Gets a node to work with" << endl;
            cout << "rnd"           << "\t\t - Remove node" << endl;
            cout << "pnd"           << "\t\t - Print all nodes" << endl;
            cout << "cnt"           << "\t\t - Add new network" << endl;
            cout << "gnt"           << "\t\t - Gets a network to work with" << endl;
            cout << "rnt"           << "\t\t - Remove networks" << endl;
            cout << "pnt"           << "\t\t - Print all networks" << endl;
            cout << "priv_dir"      << "\t - Directory with user private keys" << endl;
            cout << "pub_dir"       << "\t - Directory with user public keys" << endl;
            cout << "cert_dir"      << "\t - Directory with user certificates" << endl;
            cout << "save"          << "\t\t - Save state" << endl;
            cout << "load"          << "\t\t - Load state" << endl;
            cout << "clear"         << "\t\t - Clear data" << endl;
        }

        void await_stop() {
            signals.async_wait(
                    [this](boost::system::error_code /*ec*/, int /*signo*/) { sp2p_manager.stopAll(); });
        }

     private:

        void scanForPrivKeys(const std::string& dir) {
            vector<string> filenames;
            getFilenames(dir, filenames);

            string pass;
            boost::regex reg1("(\\w+)\\.pem");
            boost::regex reg2("(\\w+)\\.(\\w+)\\.pem");
            boost::smatch sm;
            for(string& name: filenames) {
                cout << "Type password for: " << name << endl;

                disableEnableEcho(false);
                cin >> pass;
                disableEnableEcho(true);

                if(boost::regex_match(name, sm, reg1)) {
                    sc::enc::priv_st_ptr key(new sc::enc::PrivateKeyStore());
                    key->setFilename(name);
                    key->setPath(dir);
                    key->setName(sm[1]);
                    key->setPassword(pass);
                    key->load();
                    sc::node_ptr node = sp2p_manager.getNode(sm[1]);
                    node->getMyKeys().push_back(key);
                } else if(boost::regex_match(name, sm, reg2)) {
                    sc::enc::priv_st_ptr key(new sc::enc::PrivateKeyStore());
                    key->setFilename(name);
                    key->setPath(dir);
                    key->setName(sm[1]+"."+sm[2]);
                    key->setPassword(pass);
                    key->load();
                    sc::node_ptr node = sp2p_manager.getNode(sm[1]);
                    sc::network_ptr net = sp2p_manager.getNetwork(sm[2]);
                    node->getNetworkKeys()[net->getDescription()] = key;
                }
            }
        }

        void scanForPubKeys(const std::string& dir) {
            vector<string> filenames;
            getFilenames(dir, filenames);

            boost::regex reg("(\\w+)\\.pem");
            boost::smatch sm;
            for(string& name: filenames) {
                if(boost::regex_match(name, sm, reg)) {
                    sc::enc::pub_st_ptr key(new sc::enc::PublicKeyStore());
                    key->setFilename(name);
                    key->setPath(dir);
                    key->setName(sm[1]);
                    key->load();
                    pub_keys.push_back(key);
                }
            }
        }

        void scanForCerts(const std::string& dir) {
            vector<string> filenames;
            getFilenames(dir, filenames);

            boost::regex reg1("(\\w+)\\.([0-9]+)\\.pem");
            boost::regex reg2("(\\w+)\\.(\\w+)\\.pem");
            boost::smatch sm;
            for(string& name: filenames) {
                if(regex_match(name, sm, reg1)) {
                    sc::enc::cert_st_ptr cert(new sc::enc::CertificateStore());
                    cert->setFilename(name);
                    cert->setPath(dir);
                    cert->setName(sm[1]+"."+sm[2]);
                    cert->load();
                    sc::node_ptr node = sp2p_manager.getNode(sm[1]);
                    node->getNodeCerts().push_back(cert);
                }
                else if(regex_match(name, sm, reg2)) {
                    sc::enc::cert_st_ptr cert(new sc::enc::CertificateStore());
                    cert->setFilename(name);
                    cert->setPath(dir);
                    cert->setName(sm[1]+"."+sm[2]);
                    cert->load();
                    sc::node_ptr node = sp2p_manager.getNode(sm[1]);
                    sc::network_ptr network = sp2p_manager.getNetwork(sm[2]);
                    node->getNetworkCerts()[network->getDescription()] = cert;
                }
            }
        }

        sc::enc::CryptDataManager data_manager;
        sc::Manager sp2p_manager;

        NodeContext node_c;
        NetworkContext network_c;

        sc::node_ptr current_node;
        sc::network_ptr current_network;
        boost::asio::signal_set signals;

        std::string priv_keys_dir;
        std::string pub_keys_dir;
        std::string cert_dir;

        po::variables_map vm;

        vector<sc::enc::priv_st_ptr> priv_keys;
        vector<sc::enc::pub_st_ptr> pub_keys;
        vector<sc::enc::cert_st_ptr> certs;

        static po::options_description user;
        static po::options_description my_user;
        static po::options_description node_desc;
        static po::options_description network_desc;
        static po::options_description create_node;
        static po::options_description get_node;
        static po::options_description remove_node;
        static po::options_description create_network;
        static po::options_description get_network;
        static po::options_description remove_network;
        static po::options_description save_state;
        static po::options_description load_state;
        static po::options_description clear_data;
        static po::options_description path_opts;
        static po::options_description load_conf_opts;

}; 

po::options_description Sp2pContext::user = []() {

    po::options_description opts("User description");
    opts.add_options()
        ("username,u", po::value<string>(), "username")
        ("email,e", po::value<string>(), "email");

    return opts;
}();

po::options_description Sp2pContext::my_user = []() {

    po::options_description opts("MyUser description");
    opts.add_options()
        ("password,p", po::value<string>(), "username")
        ("registered,r", po::value<bool>()->default_value(false), "is registered (0 or 1)");

    return opts;
}();

po::options_description Sp2pContext::node_desc = []() {

    po::options_description opts("Node description");
    opts.add_options()
        ("help,h", "print this message")
        ("name,n", po::value<string>(), "node name")
        ("ip-address,i", po::value<string>(), "node ip address")
        ("port,P", po::value<uint16_t>(), "port to connect node");

    return opts;
}();

po::options_description Sp2pContext::create_node("Create node");
po::options_description Sp2pContext::get_node("Go to node");
po::options_description Sp2pContext::remove_node("Remove node");

po::options_description Sp2pContext::network_desc = []() {

    po::options_description opts("Network description");
    opts.add_options()
        ("help,h", "print this message")
        ("name,n", po::value<string>(), "network name")
        ("protocol,p", po::value<string>(), "protocol name")
        ("access_rights,A", po::value<string>(), "(PUBLIC, PRIVATE)")
        ("visibility,V", po::value<string>(), "(LISTED, UNLISTED)")
        ("participation,P", po::value<string>(), "(CLIENT_ONLY, CLIENT_SERVER)");

    return opts;
}();

po::options_description Sp2pContext::create_network("Add network");
po::options_description Sp2pContext::remove_network("Remove network");
po::options_description Sp2pContext::get_network("Go to network");

po::options_description Sp2pContext::save_state = []() {

    po::options_description opts("Save state");
    opts.add_options()
        ("help,h", "print this message")
        ("filename,f", po::value<string>(), "filename to save data")
        ("password,p", po::value<string>(), "password to encode file");

    return opts;
}();

po::options_description Sp2pContext::load_state = []() {

    po::options_description opts("Load state");
    opts.add_options()
        ("help,h", "print this message")
        ("filename,f", po::value<string>(), "filename to load data from")
        ("password,p", po::value<string>(), "password to decode file");

    return opts;
}();

po::options_description Sp2pContext::path_opts = []() {
    po::options_description opts("Path");
    opts.add_options()
        ("help,h", "print this message")
        ("path,p", "set path");

    return opts;
}();

po::options_description Sp2pContext::load_conf_opts = []() -> po::options_description {

    po::options_description opts("Load configuration file");
    opts.add_options()
        ("node_name", po::value<string>(), "name of node to load")
        ("network_name", po::value<string>(), "name of network to load")
        ("priv_keys_dir", po::value<string>(), "directory with private keys")
        ("pub_keys_dir", po::value<string>(), "directory with public keys")
        ("certs_dir", po::value<string>(), "directory with certificates");

    return opts;
}();
/* ------------------ ~ Sp2pContext ------------------------------------------------- */

#endif /* CONTEXT_HPP  */
