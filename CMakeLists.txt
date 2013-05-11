cmake_minimum_required(VERSION 2.6)
project(sp2p)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules )

add_subdirectory(sercli)
add_subdirectory(tracker)
