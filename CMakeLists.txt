cmake_minimum_required(VERSION 3.0)

project(TICKET_SYSTEM)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(BP_tree)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(code main.cpp)