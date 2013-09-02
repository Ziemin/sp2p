include(FindProtobuf)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Boost COMPONENTS thread)
find_package(Boost COMPONENTS date_time)
find_package(Threads)
find_package(Botan REQUIRED)
find_package(Protobuf REQUIRED)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})

add_library(Sercli STATIC
	user.cpp
	node.cpp
	#connection.cpp
	#connection_manager.cpp
	sp2p_protocol.pb.cc
	nodemessage.cpp
	parser.cpp
	handler.cpp
	globals.cpp
	manager.cpp
	nodeconnection.cpp
	message_factory.cpp
	network.cpp
	server.cpp
)

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)

set(LOG OFF CACHE BOOL "Enable logging")

if(LOG)
	message("-- Logging enabled")
	add_definitions(-DLOGGING_ENABLED)
else(LOG)
	message("-- Logging disabled")
endif(LOG)
