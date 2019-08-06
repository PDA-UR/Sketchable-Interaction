cmake_minimum_required(VERSION 3.13)
project(SIRenderEngine)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIS})

set(SIRENDERENGINESrc
        src/render_engine/sdl2/SIRenderEngineSDL2.cpp
        src/render_engine/sdl2/SIRenderEngineSDL2.hpp
        src/render_engine/sdl2/region_sprite/RegionSprite.c
        src/render_engine/sdl2/region_sprite/RegionSprite.h
        src/render_engine/sdl2/shader/GLSLProgram.c
        src/render_engine/sdl2/shader/GLSLProgram.h)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})
target_link_libraries(SIRenderEngine ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES} SIRuntime)
add_dependencies(SIRenderEngine SIRuntime)