cmake_minimum_required(VERSION 3.11)

project(hope-core-main)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)
add_subdirectory(lib)