cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SI LIBRARY
add_library(SI SHARED
        src/core/Core.cpp
        src/core/Core.hpp
        src/SI.cpp
        src/SI.hpp)

# SI TEST APPLICATION
add_executable(si_test_application test/app/src/main.cpp)
target_link_libraries(si_test_application SI)

# SI GTEST UNIT TEST RUNNER
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include)

add_subdirectory(../googletest googletest)

add_executable(unit_tests_run test/unit_tests/src/unit_test_run.cpp test/unit_tests/src/SI_test.cpp test/unit_tests/src/SI_test.hpp)
target_link_libraries(unit_tests_run SI gtest gtest_main gmock gmock_main pthread)