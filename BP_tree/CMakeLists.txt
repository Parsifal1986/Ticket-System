cmake_minimum_required(VERSION 3.0)

project(BP_TEST)

include_directories(bp_tree.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main main.cpp)