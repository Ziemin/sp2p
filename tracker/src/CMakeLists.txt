add_subdirectory(tests)

find_package(Boost 1.50 COMPONENTS system REQUIRED)
find_package(Boost 1.50 COMPONENTS thread)
find_package(Boost 1.50 COMPONENTS date_time)
find_package(Threads)
find_package(Botan REQUIRED)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR} )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/tracker/build)
add_executable(tracker main.cpp)

#add_library(trackerLib STATIC
	##libs
#)

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)

#target_link_libraries (tracker trackerLib)
target_link_libraries (tracker ${BOTAN_LIBRARIES})
target_link_libraries (tracker ${Boost_LIBRARIES})
target_link_libraries (tracker ${Boost_SYSTEM_LIBRARY})
target_link_libraries (tracker ${CMAKE_THREAD_LIBS_INIT})


