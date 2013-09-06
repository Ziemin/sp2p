cmake_minimum_required(VERSION 2.6)
project(node_client)

include(FindProtobuf)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules )

SET(CMAKE_CXX_COMPILER "/usr/bin/clang")

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-Wall -std=c++11")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

#SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
#SET (CMAKE_NM      "/usr/bin/llvm-nm")
#SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
#SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${node_client_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${node_client_SOURCE_DIR}/lib)

# SOURCES
add_subdirectory(src)

# TESTS
add_subdirectory(test)
