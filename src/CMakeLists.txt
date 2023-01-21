cmake_minimum_required(VERSION 3.8)

set(This server)
set(Exe ${This}_main)

project(${This} CXX)

set(Headers
    server.hpp
)

set(Sources
    server.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})

add_executable(${Exe} ${Sources} main.cpp ${Headers})
target_link_libraries(${Exe} ${This})
