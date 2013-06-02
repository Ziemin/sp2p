add_subdirectory(tests)

find_package(Boost 1.50 COMPONENTS system REQUIRED)
find_package(Boost 1.50 COMPONENTS thread)
find_package(Boost 1.50 COMPONENTS date_time)
find_package(Threads)
find_package(Botan REQUIRED)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/sercli/build)
add_executable(sercli main.cpp)

add_library(sercliLib STATIC
	user.cpp
	node.cpp
)

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)

#target_link_libraries (sercli sercliLib)
target_link_libraries (sercli ${BOTAN_LIBRARIES})
target_link_libraries (sercli ${Boost_LIBRARIES})
target_link_libraries (sercli ${Boost_SYSTEM_LIBRARY})
target_link_libraries (sercli ${CMAKE_THREAD_LIBS_INIT})


