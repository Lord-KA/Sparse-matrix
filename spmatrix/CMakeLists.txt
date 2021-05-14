cmake_minimum_required(VERSION 3.17)
project(SPMatrix)


add_executable(treap test-treap.cpp treap.hpp)
add_executable(spmatrix spmatrix.hpp test-spmatrix.cpp treap.hpp Matrix.hpp)

target_link_libraries(
    spmatrix
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(spmatrix)
