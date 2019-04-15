cmake_minimum_required(VERSION 3.13)
project(engine_scripting)

set(CMAKE_CXX_STANDARD 11)

find_package(PythonLibs 3 REQUIRED)
find_package(Python3)
find_package(Boost 1.69.0 COMPONENTS system python3.7 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories( ${PYTHON_INCLUDE_DIRS})
add_definitions(-DQT_NO_KEYWORDS)

add_library(engine_scripting_lib MODULE src/region/region.cpp src/region/region.h)
target_link_libraries(engine_scripting_lib ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

add_executable(engine_scripting src/main.cpp src/region/region.cpp src/region/region.h src/engine/engine.cpp src/engine/engine.h src/engine/timing.h src/scripting/scripting.cpp src/scripting/scripting.h src/scripting/plugins_collector.cpp src/scripting/plugins_collector.h)
target_link_libraries(engine_scripting ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(engine_scripting engine_scripting_lib)

add_custom_command(TARGET engine_scripting_lib POST_BUILD COMMAND cp ${CMAKE_BINARY_DIR}/libengine_scripting_lib.so ${CMAKE_SOURCE_DIR}/lib)
