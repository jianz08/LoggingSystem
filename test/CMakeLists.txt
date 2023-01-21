cmake_minimum_required(VERSION 3.8)

# --------------------------------------------------
# Set up GoogleTest
# --------------------------------------------------

# Include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --------------------------------------------------

set(This serverTests)
include_directories(../src)

set(Sources
    serverTests.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
    server
)

# add_test(
#     NAME ${This}
#     COMMAND ${This}
# )

# Enable CMake's test runner to discover the tests included in the binary
include(GoogleTest)
gtest_discover_tests(${This})
