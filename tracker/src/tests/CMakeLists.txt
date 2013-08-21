find_package(Boost COMPONENTS system REQUIRED)
find_package(Boost COMPONENTS thread)
find_package(Boost COMPONENTS date_time)
find_package(Boost COMPONENTS unit_test_framework)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/tracker/build/tests)

add_executable(tracker_inittest tracker_inittest.cpp)

target_link_libraries (tracker_inittest ${Boost_LIBRARIES})
target_link_libraries (tracker_inittest ${CMAKE_THREAD_LIBS_INIT})
