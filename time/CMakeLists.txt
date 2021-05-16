cmake_minimum_required(VERSION 3.17)
project(Time)


add_executable(time test-time.cpp time.hpp time.cpp)

target_link_libraries(
    time
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(time)
