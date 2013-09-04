include(FindProtobuf)
find_package(Boost COMPONENTS system thread date_time serialization unit_test_framework REQUIRED)
find_package(Threads REQUIRED)
find_package(Botan REQUIRED)
find_package(Protobuf REQUIRED)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

ENABLE_TESTING()

set(LOG_TEST OFF CACHE BOOL "Enable logging")

if(LOG_TEST)
	message("-- Logging enabled in testing")
	add_definitions(-DLOGGING_ENABLED)
else(LOG_TEST)
	message("-- Logging disabled in testing")
endif(LOG_TEST)

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/sercli/build/tests)

SET(TEST_PROPS -log_level=all)

message("-- Adding test files:")
file(GLOB TEST_FILES "*.cpp")
set(TEST_NAMES "")
message(${Boost_LIBRARIES})
message(${Boost_INCLUDE_DIRS})
foreach(file ${TEST_FILES})

    get_filename_component(base_name ${file} NAME_WE)
    get_filename_component(full_name ${file} NAME)

    add_executable(${base_name} ${full_name})

    target_link_libraries (${base_name} ${Boost_LIBRARIES})
    target_link_libraries (${base_name} ${Boost_SYSTEM_LIBRARY})
    target_link_libraries (${base_name} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries (${base_name} ${BOTAN_LIBRARIES})
    target_link_libraries (${base_name} ${PROTOBUF_LIBRARY})

    target_link_libraries (${base_name} ${LIBRARY_OUTPUT_PATH}/libSercli.a)

    add_test(NAME ${base_name} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${base_name} ${TEST_PROPS})
    set_tests_properties(${base_name} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

    LIST(APPEND TEST_NAMES ${base_name})
endforeach()

message(${LIBRARY_OUTPUT_PATH})
message("--   Tests found:")
foreach(test ${TEST_NAMES})
    message("--    "${test})
endforeach()

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TEST_NAMES})
