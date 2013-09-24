sp2p
====

secure peer to peer


// --------------- SERVER ------------------------------------------ //
server instalation:

server config in tracker/qmake_project/constants.hpp
database config in tracker/qmake_project/db/dbconfig.h

Setup postgres db with database/db_init.sql

To compile the server go to tracker/qmake_project and run:
$ qmake tracker.pro -makefile
$ make

To run it:
$ ./tracker

// --------------- NODE CLIENT ------------------------------------- // 

Node_client is a simple application presenting usage of sp2p protocol. 
It makes use of nearly all features of sercli library.

To use application library has to be built first, what requires 
boost 1.54, botan 1.10, openSSL, clang 3.3 and cmake 2.8 installed.

To build library go to /sercli and do:
cmake .
make

To build node_client go to /node_client and do the same

Executable is created in node_client/build directory (/node_client/nodeClient)

To get familiar with triggering node_client run:
nodeClient -h

Application needs a path to configuration file, example of which is
accessible in node_client directory (sp2p.conf)
There you specify priv_keys_dir with private keys, pub_keys_dir and certs_dir by analogy.
node_name and network_name are optional. Moreover application wont run when these keys are set 
to values not existing in its context.

To get help and brief description of all functions type help when app is running.
If you want to work with specific node and network you have to use gnt and gnd 
commands to set network and node in context. Typing cd Node will move you to 
another context with specific functions (again help will tell you about them)
Some of the function just by writing "function_name" -h will print out a help message
on console.

To avoid typing gnt and gnd every time the app is started to set node and 
network to work with, you can set them in advance in config file - vide node_name and
network_name placeholders. Still the requirement is to have them saved in data file.



