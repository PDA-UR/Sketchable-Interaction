cmake_minimum_required(VERSION 3.13)
project(SIRenderEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

set(SIRENDERENGINESrc
        src/render_engine/sdl2/SIRenderEngineSDL2.cpp
        src/render_engine/sdl2/SIRenderEngineSDL2.hpp)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})
target_link_libraries(SIRenderEngine ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} SIRuntime)
add_dependencies(SIRenderEngine SIRuntime)