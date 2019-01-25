cmake_minimum_required(VERSION 3.13)
project(core)

set(CMAKE_CXX_STANDARD 14)

SET(SRCS
        si.cpp
        si.h
        si_debug/debug.h
        si_engine/engine.cpp
        si_engine/engine.h
        si_engine/signal_handler.cpp
        si_engine/signal_handler.h
        si_plugin/plugin.cpp
        si_plugin/plugin.h
        si_region/region.cpp
        si_region/region.h si_engine/step.cpp si_engine/step.h si_region/region_template.cpp si_region/region_template.h)

SET(SRCS_TEST si_test/main.cpp)

find_package(Qt5Widgets REQUIRED)

add_library(si-core SHARED ${SRCS})
set_target_properties(si-core PROPERTIES AUTOMOC TRUE)
target_link_libraries(si-core Qt5::Widgets)
add_executable(si_engine_test ${SRCS_TEST})
target_link_libraries(si_engine_test si-core)