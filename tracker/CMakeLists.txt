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

include(FindProtobuf)

add_definitions(-BOOST_ASIO_ENABLE_HANDLER_TRACKING)

add_subdirectory(src)

ENABLE_TESTING()
ADD_TEST(tracker_inittest build/tests/tracker_inittest)
