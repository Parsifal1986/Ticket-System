cmake_minimum_required(VERSION 3.0)

project(BP_TEST)
set(CMAKE_CXX_STANDARD 23)

include_directories(bp_tree.hpp Exceptions.hpp LRU.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bpt_bin)

add_executable(BP_TEST main.cpp)
# add_executable(LRUTEST LRUTEST.cpp)