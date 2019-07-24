cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PythonLibs 3 REQUIRED)
find_package(Python3)
find_package(Boost 1.69.0 COMPONENTS system python3.7)

include_directories(${Boost_INCLUDE_DIRS})
include_directories( ${PYTHON_INCLUDE_DIRS})

# add_definitions(-DQT_NO_KEYWORDS)

set(xlib TRUE)
if(xlib)
    add_definitions(-Dxlib)
endif()

set(PySISrc
        src/python3_plugin/Plugin.cpp
        src/python3_plugin/Plugin.hpp)

# SI PYTHON3 MODULE
add_library(PySI MODULE ${PySISrc})

target_link_libraries(PySI ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# SI LIBRARY

set(SISrc
        src/core/Core.cpp
        src/core/Core.hpp
        src/SIRuntime.cpp
        src/SIRuntime.hpp
        src/python3_plugin/Plugin.cpp
        src/python3_plugin/Plugin.hpp
        src/python3_plugin/Scripting.cpp
        src/python3_plugin/Scripting.hpp
        src/core/region/Region.cpp
        src/core/region/Region.hpp
        src/debug/Print.hpp
        src/python3_plugin/PluginCollector.cpp
        src/python3_plugin/PluginCollector.hpp
        src/core/collision/CollisionMask.cpp
        src/core/collision/CollisionMask.hpp
        src/core/collision/CollisionManager.cpp
        src/core/collision/CollisionManager.hpp
        include/SI/SI.hpp
        src/context/Context.cpp
        src/context/Context.hpp
        src/context/LayerManager.cpp
        src/context/LayerManager.hpp
        src/context/Layer.cpp
        src/context/Layer.hpp)

add_library(SI SHARED ${SISrc})

if (xlib)
    target_link_libraries(SI ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} -lX11)
else()
    target_link_libraries(SI ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
endif()

add_dependencies(SI PySI)

# SI TEST APPLICATION
set(SITestAppSrc
        examples/app/src/main.cpp)

add_executable(si_test_application ${SITestAppSrc})
target_link_libraries(si_test_application SI)
add_dependencies(si_test_application PySI)
add_dependencies(si_test_application SI)


# SI GTEST UNIT TEST RUNNER
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include)

add_subdirectory(../googletest googletest)

add_executable(unit_tests_run tests/src/unit_test_run.cpp
        tests/src/SIRuntime_test.cpp
        tests/src/include/SIRuntime_test.hpp
        tests/src/SiPluginTest.cpp
        tests/src/include/SiPluginTest.hpp
        tests/src/SiScriptingTest.cpp
        tests/src/include/SiScriptingTest.hpp
        tests/src/SiPluginCollectorTest.cpp
        tests/src/include/SiPluginCollectorTest.hpp
        tests/src/SiPrintTest.cpp
        tests/src/include/SiPrintTest.hpp)

target_link_libraries(unit_tests_run ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} SI gtest gtest_main gmock gmock_main pthread -ldl -lutil)
add_dependencies(unit_tests_run PySI)
add_dependencies(unit_tests_run SI)


#add_custom_command(TARGET PySI POST_BUILD COMMAND cp ${CMAKE_SOURCE_DIR}/build/release/libPySI.so ${CMAKE_SOURCE_DIR}/libs)
#add_custom_command(TARGET SI POST_BUILD COMMAND cp ${CMAKE_SOURCE_DIR}/build/release/libSI.so ${CMAKE_SOURCE_DIR}/libs)
