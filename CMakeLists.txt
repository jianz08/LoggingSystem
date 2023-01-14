cmake_minimum_required(VERSION 3.8)

set(This server)

project(${This} CXX)

set(CMAKE_CXX_STANDAND 17)
add_compile_options("-std=c++17")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Headers
    server.hpp
)

set(Sources
    server.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})
#add_executable(${This} ${Sources} ${Headers})

add_subdirectory(test)