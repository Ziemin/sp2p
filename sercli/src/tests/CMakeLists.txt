find_package(Boost COMPONENTS system REQUIRED)
find_package(Boost COMPONENTS thread)
find_package(Boost COMPONENTS date_time)
find_package(Boost COMPONENTS unit_test_framework)
find_package(Botan REQUIRED)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/sercli/build/tests)

add_executable(sercli_inittest sercli_inittest.cpp)
add_executable(sercli_edu_tests sercli_edu_tests.cpp)

target_link_libraries (sercli_inittest ${Boost_LIBRARIES})
target_link_libraries (sercli_inittest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (sercli_inittest ${BOTAN_LIBRARIES})
target_link_libraries (sercli_inittest ${Boost_SYSTEM_LIBRARY})

target_link_libraries (sercli_edu_tests ${Boost_LIBRARIES})
target_link_libraries (sercli_edu_tests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (sercli_edu_tests ${BOTAN_LIBRARIES})
target_link_libraries (sercli_edu_tests ${Boost_SYSTEM_LIBRARY})
