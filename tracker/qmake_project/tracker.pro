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
    utils/util_functions.cpp \
    utils/proto_invitation.cpp \
    utils/proto_network.cpp \
    utils/proto_server.cpp \
    utils/proto_user.cpp \
    request.cpp \
    response.cpp \
    protocolfactory.cpp \
    logincontroler.cpp

unix|win32: LIBS +=

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
    utils/util_functions.h \
    utils/proto_invitation.hpp \
    utils/proto_network.hpp \
    utils/proto_server.hpp \
    utils/proto_user.hpp \
    types.hpp \
    request.hpp \
    response.hpp \
    protocol_factory/abstractprotocolfactory.hpp \
    protocol_factory/abstractrequest.hpp \
    protocol_factory/abstractresponse.hpp \
    protocol_factory/abstractrequestparser.hpp \
    protocol_factory/abstractrequesthandler.hpp \
    protocolfactory.hpp \
    logincontroler.hpp \
    protocol_factory/abstractlogincontroler.hpp

QMAKE_CXXFLAGS = -std=c++11

unix|win32: LIBS += -lpqxx -lboost_system -lboost_thread -lprotobuf -lpthread
