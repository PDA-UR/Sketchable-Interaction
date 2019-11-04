cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SITest)

find_package(PythonLibs 3 REQUIRED)
find_package(Python3 REQUIRED)
find_package(Boost 1.69.0 COMPONENTS system python3.7 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

# SI GTEST UNIT TEST RUNNER
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include)

add_subdirectory(googletest googletest)

set(SIUnitTestSrc
        SI/tests/src/run_unit_tests.cpp
        SI/tests/src/SIGRunTest.cpp
        SI/tests/src/SIGRunTest.hpp
        SI/tests/src/SIGRunCoreTest.cpp
        SI/tests/src/SIGRunCoreTest.hpp
        SI/tests/src/SIGRunPrintTest.cpp
        SI/tests/src/SIGRunPrintTest.hpp)

add_executable(run_unit_tests ${SIUnitTestSrc})
target_link_libraries(run_unit_tests ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} gtest gtest_main gmock gmock_main pthread -ldl -lutil SIGeneralRuntime)
add_dependencies(run_unit_tests SIGeneralRuntime)
