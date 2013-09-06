link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})
find_package(Boost COMPONENTS system thread date_time serialization unit_test_framework log log_setup REQUIRED)
find_package(Threads REQUIRED)
find_package(Botan REQUIRED)
find_package(Protobuf REQUIRED)

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)

add_library(Sercli STATIC
	user.cpp
	node.cpp
	sp2p_protocol.pb.cc
	nodemessage.cpp
	parser.cpp
	handler.cpp
	globals.cpp
	manager.cpp
	nodeconnection.cpp
	message_factory.cpp
	network.cpp
	parser_factory.cpp
	data.cpp
	logging.cpp
	sp2p_types.cpp
)

set(LOG OFF CACHE BOOL "Enable logging")

if(LOG)
	message("-- Logging enabled")
	add_definitions(-DLOGGING_ENABLED)
else(LOG)
	message("-- Logging disabled")
endif(LOG)
