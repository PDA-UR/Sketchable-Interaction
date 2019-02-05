cmake_minimum_required(VERSION 3.13)
project(core)

set(CMAKE_CXX_STANDARD 14)

SET(SRC
        src/si.cpp
        include/si.h
        include/debug.h
        src/engine.cpp
        include/engine.h
        src/signal_handler.cpp
        include/signal_handler.h
        src/region.cpp
        include/region.h
        src/step.cpp
        include/step.h
        src/mask.cpp
        include/mask.h
        src/effect.cpp
        include/effect.h
        src/canvas_region.cpp
        include/canvas_region.h
        src/mouse_region.cpp
        include/mouse_region.h
        include/util.h)

SET(SRC_TEST_APPLICATION
        src/main.cpp)

SET(SRC_GTEST_RUNNER
        test/src/unit_tests_run.cpp
        test/src/si_test.cpp
        test/include/si_test.h
        test/include/cmd_args.h
        test/src/si_engine_test.cpp
        test/include/si_engine_test.h
        test/src/QComponentTestrunner.cpp
        test/include/QComponentTestrunner.h)

# SI CORE LIBRARY
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)
add_library(si-core SHARED ${SRC})
set_target_properties(si-core PROPERTIES AUTOMOC TRUE)
target_link_libraries(si-core Qt5::Widgets)


# SI TESTING APPLICATION
add_executable(si_engine_test ${SRC_TEST_APPLICATION})
target_link_libraries(si_engine_test si-core)

# SI GTEST UNIT TEST RUNNER
include_directories(${GTEST_INCLUDE_DIRS})
find_package(GTest REQUIRED)
add_executable(unit_tests_run ${SRC_GTEST_RUNNER})
set_target_properties(unit_tests_run PROPERTIES AUTOMOC TRUE)
target_link_libraries(unit_tests_run Qt5::Widgets Qt5::Test si-core gtest gtest_main pthread)