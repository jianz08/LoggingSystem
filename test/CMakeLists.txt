cmake_minimum_required(VERSION 3.8)

set(This serverTests)

set(Sources
    serverTests.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
    server
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

