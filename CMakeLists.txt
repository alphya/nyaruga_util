cmake_minimum_required(VERSION 2.8)

project(nyaruga_util CXX)

add_definitions(-std=c++17 -O3 -Wall -Wextra -pedantic)

include_directories(./)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(test)

enable_testing()
