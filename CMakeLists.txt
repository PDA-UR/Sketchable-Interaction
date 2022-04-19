cmake_minimum_required(VERSION 3.16)
project(SI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(BUILD_DOCS FALSE)

#SIEngine
include(SI/src/CMakeLists.txt)
include(SI/src/siren/CMakeLists.txt)
include(SI/src/sirose/CMakeLists.txt)

#SItools
include(SI/src/sitools/CMakeLists.txt)

#SIEgen
include(SI/src/e/CMakeLists.txt)

# SIExamples
include(SI/examples/app/CMakeLists.txt)

# SITests
include(SI/tests/CMakeLists.txt)

