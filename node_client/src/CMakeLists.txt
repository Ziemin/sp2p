link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})
find_package(Boost COMPONENTS system thread date_time serialization unit_test_framework log log_setup REQUIRED)
find_package(Threads REQUIRED)
find_package(Botan REQUIRED)
find_package(Protobuf REQUIRED)

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)

SET(SERCLI_LIB_PATH ${node_client_SOURCE_DIR}/../sercli/lib)

add_library(nodeClientLib STATIC
    #empty 
)

add_executable(nodeClient main.cpp)

target_link_libraries (nodeClient ${Boost_LIBRARIES})
target_link_libraries (nodeClient ${Boost_SYSTEM_LIBRARY})
target_link_libraries (nodeClient ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (nodeClient ${BOTAN_LIBRARIES})
target_link_libraries (nodeClient ${PROTOBUF_LIBRARY})
target_link_libraries (nodeClient ${SERCLI_LIB_PATH}/libSercli.a)
target_link_libraries (nodeClient nodeClientLib)

set(LOG OFF CACHE BOOL "Enable logging")

if(LOG)
	message("-- Logging enabled")
	add_definitions(-DLOGGING_ENABLED)
else(LOG)
	message("-- Logging disabled")
endif(LOG)
