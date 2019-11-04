cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#SIEngine
include(SI/src/CMakeLists.txt)

# SIExamples
include(SI/examples/app/CMakeLists.txt)

# SITests
include(SI/tests/CMakeLists.txt)
