cmake_minimum_required(VERSION 3.17)
project(YearFromAdam)


add_executable(yearfa test-yearfa.cpp yearfa.cpp yearfa.hpp)

target_link_libraries(
    yearfa
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(yearfa)
