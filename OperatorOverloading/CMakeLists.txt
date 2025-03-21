cmake_minimum_required(VERSION 3.16)
project(OperatorOverloading)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(OperatorOverloading ${SRC_FILES})
