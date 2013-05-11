find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(Boost 1.50 COMPONENTS system REQUIRED)
find_package(Boost 1.50 COMPONENTS thread)
find_package(Boost 1.50 COMPONENTS date_time)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/sercli/build/tests)

add_executable(sercli_inittest sercli_inittest.cpp)

target_link_libraries (sercli_inittest ${Boost_LIBRARIES})
target_link_libraries (sercli_inittest ${CMAKE_THREAD_LIBS_INIT})
