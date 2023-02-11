cmake_minimum_required(VERSION 3.8)

project(logging CXX)

set(CMAKE_CXX_STANDAND 17)
add_compile_options("-std=c++17")
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)