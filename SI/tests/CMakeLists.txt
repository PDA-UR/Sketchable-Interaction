cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(SITest)

find_package(PythonLibs 3 REQUIRED)
find_package(Python3 REQUIRED)
find_package(Boost 1.69.0 COMPONENTS system python3.7 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SI/src)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

# SI GTEST UNIT TEST RUNNER
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include)

add_subdirectory(googletest googletest)

set(SIUnitTestSrc
        SI/tests/src/run_unit_tests.cpp
        SI/tests/src/sigrun/SIGRunTest.cpp
        SI/tests/src/sigrun/SIGRunTest.hpp
        SI/tests/src/sigrun/SIGRunCoreTest.cpp
        SI/tests/src/sigrun/SIGRunCoreTest.hpp
        SI/tests/src/sigrun/debug/SIGRunPrintTest.cpp
        SI/tests/src/sigrun/debug/SIGRunPrintTest.hpp
        SI/tests/src/sigrun/plugin/SIGRunPluginCollectorTest.hpp
        SI/tests/src/sigrun/plugin/SIGRunPluginCollectorTest.cpp
        SI/tests/src/sigrun/plugin/SIGRunScriptingTest.cpp
        SI/tests/src/sigrun/plugin/SIGRunScriptingTest.hpp
        SI/tests/src/sigrun/log/SIGRunLogTest.cpp
        SI/tests/src/sigrun/log/SIGRunLogTest.hpp
        SI/tests/src/sigrun/region/SIGRunRegionTransformTest.cpp
        SI/tests/src/sigrun/region/SIGRunRegionTransformTest.hpp
        SI/tests/src/sigrun/region/SIGRunRegionMaskTest.cpp
        SI/tests/src/sigrun/region/SIGRunRegionMaskTest.hpp
        SI/tests/src/sigrun/region/SIGRunRegionTest.cpp
        SI/tests/src/sigrun/region/SIGRunRegionTest.hpp
        SI/tests/src/sigrun/context/managers/SIGRunCollisionManagerTest.cpp
        SI/tests/src/sigrun/context/managers/SIGRunCollisionManagerTest.hpp
        SI/tests/src/sigrun/context/SIGRunContextTest.cpp
        SI/tests/src/sigrun/context/SIGRunContextTest.hpp
        SI/tests/src/sigrun/region/SIGRunRegionResamplerTest.cpp
        SI/tests/src/sigrun/region/SIGRunRegionResamplerTest.hpp
        SI/tests/src/sigrun/context/managers/SIGRunRegionManagerTest.cpp
        SI/tests/src/sigrun/context/managers/SIGRunRegionManagerTest.hpp
        SI/tests/src/sigrun/context/managers/SIGRunLinkingManagerTest.cpp
        SI/tests/src/sigrun/context/managers/SIGRunLinkingManagerTest.hpp
        SI/tests/src/sigrun/context/managers/SIGRunExternalApplicationManagerTest.cpp
        SI/tests/src/sigrun/context/managers/SIGRunExternalApplicationManagerTest.hpp
        SI/tests/src/sigrun/util/SIGRunRingBufferTest.cpp
        SI/tests/src/sigrun/util/SIGRunRingBufferTest.hpp
        SI/tests/src/sigrun/util/SIGRunUtilTest.cpp
        SI/tests/src/sigrun/util/SIGRunUtilTest.hpp
        SI/tests/src/pysi/stl_container_exposure/PySIVectorExposureTest.cpp
        SI/tests/src/pysi/stl_container_exposure/PySIVectorExposureTest.hpp
        SI/tests/src/pysi/stl_container_exposure/PySIMapExposureTest.cpp
        SI/tests/src/pysi/stl_container_exposure/PySIMapExposureTest.hpp
        SI/tests/src/pysi/PySIPySIEffectTest.cpp
        SI/tests/src/pysi/PySIPySIEffectTest.hpp
        SI/tests/src/sigrun/context/managers/helpers/input/SIGRunExternalObjectTest.cpp
        SI/tests/src/sigrun/context/managers/helpers/input/SIGRunExternalObjectTest.hpp
        SI/tests/src/sigrun/context/managers/helpers/input/SIGRunFilesystemTest.cpp
        SI/tests/src/sigrun/context/managers/helpers/input/SIGRunFilesystemTest.hpp
        SI/tests/src/sigrun/context/managers/helpers/linking/SIGRunLinkCandidateTest.cpp
        SI/tests/src/sigrun/context/managers/helpers/linking/SIGRunLinkCandidateTest.hpp
        SI/tests/src/sigrun/context/managers/helpers/linking/SIGRunLinkTest.cpp
        SI/tests/src/sigrun/context/managers/helpers/linking/SIGRunLinkTest.hpp
        )

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

add_executable(run_unit_tests ${SIUnitTestSrc})

target_precompile_headers(run_unit_tests
        PUBLIC

        PRIVATE
        <unordered_map>
        <vector>
        <boost/python.hpp>
        <QApplication>
        <QMainWindow>
        <QWindow>
        <QProcess>
        <QObject>
        <QSignalSpy>
        <gtest/gtest.h>
        <gmock/gmock.h>
        )

set_target_properties(run_unit_tests PROPERTIES AUTOMOC TRUE)

target_link_libraries(run_unit_tests Qt5::Widgets Qt5::Test ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} gtest gtest_main gmock gmock_main pthread -ltbb -ldl -lutil SIGeneralRuntime)
add_dependencies(run_unit_tests PySI SIGeneralRuntime)
