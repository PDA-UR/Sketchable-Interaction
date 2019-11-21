cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(BUILD_DOCS TRUE)

#SIEngine
include(SI/src/CMakeLists.txt)

# SIExamples
include(SI/examples/app/CMakeLists.txt)

# SITests
include(SI/tests/CMakeLists.txt)

