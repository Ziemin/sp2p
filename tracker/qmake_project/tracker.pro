TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    requesthandler.cpp \
    requestparser.cpp \
    server.cpp \
    connection.cpp \
    proto/sp2p_protocol.pb.cc \
    proto/nodemessagefactory.cpp \
    db/psqlconnector.cpp \
    utils/user.cpp \
    utils/network.cpp \
    utils/server.cpp \
    utils/invitation.cpp \
    utils/util_functions.cpp

unix|win32: LIBS += -lboost_system

HEADERS += \
    requestparser.hpp \
    requesthandler.hpp \
    server.hpp \
    connection.hpp \
    proto/sp2p_protocol.pb.h \
    proto/nodemessagefactory.h \
    db/psqlconnector.h \
    db/dbresponse.h \
    db/dbconfig.h \
    db/abstractdbconnector.h \
    utils/user.hpp \
    utils/network.hpp \
    utils/server.hpp \
    utils/invitation.hpp \
    utils/util_functions.h
