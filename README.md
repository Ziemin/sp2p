sp2p
====

secure peer to peer


server instalation:

server config in tracker/qmake_project/constants.hpp
database config in tracker/qmake_project/db/dbconfig.h

Setup postgres db with database/db_init.sql

To compile the server go to tracker/qmake_project and run:
$ qmake tracker.pro -makefile
$ make

To run it:
$ ./tracker
