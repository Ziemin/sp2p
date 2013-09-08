cmake_minimum_required(VERSION 2.6)
project(sp2p)

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

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sp2p_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${sp2p_SOURCE_DIR}/lib)

# SOURCES
add_subdirectory(src)

# TESTS
add_subdirectory(test)
