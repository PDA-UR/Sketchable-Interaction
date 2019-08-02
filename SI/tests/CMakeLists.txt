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

add_subdirectory(../googletest googletest)

set(SIUnitTestSrc
        tests/src/unit_test_run.cpp
        tests/src/SIEngineTest.cpp
        tests/src/include/SIEngineTest.hpp
        tests/src/SiPluginTest.cpp
        tests/src/include/SiPluginTest.hpp
        tests/src/SiScriptingTest.cpp
        tests/src/include/SiScriptingTest.hpp
        tests/src/SiPluginCollectorTest.cpp
        tests/src/include/SiPluginCollectorTest.hpp
        tests/src/SiPrintTest.cpp
        tests/src/include/SiPrintTest.hpp
        tests/src/SiLayerManagerTest.cpp
        tests/src/include/SiLayerManagerTest.hpp
        tests/src/SiPluginManagerTest.cpp
        tests/src/include/SiPluginManagerTest.hpp
        tests/src/SiRegionManagerTest.cpp
        tests/src/include/SiRegionManagerTest.hpp
        tests/src/SiMathTest.cpp
        tests/src/include/SiMathTest.hpp
        tests/src/SiCollisionMaskTest.cpp
        tests/src/include/SiCollisionMaskTest.hpp)

add_executable(unit_tests_run ${SIUnitTestSrc})
target_link_libraries(unit_tests_run ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} SIRenderEngine SIRuntime gtest gtest_main gmock gmock_main pthread -ldl -lutil)
add_dependencies(unit_tests_run PySI)
add_dependencies(unit_tests_run SIRuntime)
add_dependencies(unit_tests_run SIRenderEngine)