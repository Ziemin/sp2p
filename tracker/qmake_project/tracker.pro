TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    requesthandler.cpp \
    requestparser.cpp \
    server.cpp \
    connection.cpp

unix|win32: LIBS += -lboost_system

HEADERS += \
    requestparser.hpp \
    requesthandler.hpp \
    server.hpp \
    connection.hpp
