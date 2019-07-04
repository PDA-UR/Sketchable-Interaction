cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 11)

add_library(SI SHARED
        src/core/Core.cpp
        src/core/Core.hpp
        src/SI.cpp
        src/SI.hpp test/src/unit_test_run.cpp test/src/SI_test.cpp test/src/SI_test.hpp)

# SI GTEST UNIT TEST RUNNER
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include)

add_subdirectory(../googletest googletest)

add_executable(unit_tests_run test/src/unit_test_run.cpp)
target_link_libraries(unit_tests_run SI gtest gtest_main gmock gmock_main pthread)