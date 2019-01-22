cmake_minimum_required(VERSION 3.13)
project(core)

set(CMAKE_CXX_STANDARD 14)

SET(SRCS si.cpp
        si.h
        si_debug/debug.cpp
        si_debug/debug.h
        si_engine/engine.cpp
        si_engine/engine.h
        si_engine/timing.h)

add_library(si-core SHARED ${SRCS})