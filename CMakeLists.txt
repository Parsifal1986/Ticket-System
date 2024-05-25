cmake_minimum_required(VERSION 3.0)

project(TICKET_SYSTEM)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(utilities/bp_tree)

# set(SANITIZE_FLAGS "-fsanitize=address")

# add_compile_options(${SANITIZE_FLAGS})
# add_link_options(${SANITIZE_FLAGS})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(SOURCE_FILES
  "${CMAKE_SOURCE_DIR}/utilities/input_processer/input_processer.cpp"
  "${CMAKE_SOURCE_DIR}/utilities/time_system/time_system.cpp"
  "${CMAKE_SOURCE_DIR}/utilities/train_manager/train_manager.cpp"
  "${CMAKE_SOURCE_DIR}/utilities/user_manager/user_manager.cpp"
  "${CMAKE_SOURCE_DIR}/utilities/selling_system/selling_system.cpp"
  "${CMAKE_SOURCE_DIR}/utilities/log_system/log_system.cpp"
)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(code main.cpp ${SOURCE_FILES})