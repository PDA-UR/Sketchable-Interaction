cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#SIEngine
include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)

# SIExample
include(${CMAKE_CURRENT_SOURCE_DIR}/examples/app/CMakeLists.txt)

# SITests
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
