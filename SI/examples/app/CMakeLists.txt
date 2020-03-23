cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SIExample)

set(SITestAppSrc
        SI/examples/app/src/main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SI/include/)

add_executable(si_test_application ${SITestAppSrc})
target_link_libraries(si_test_application SIGeneralRuntime SIRenderEngine)
add_dependencies(si_test_application SIGeneralRuntime)

