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
        src/render_engine/sdl2/region_sprite/RegionSprite.cpp
        src/render_engine/sdl2/region_sprite/RegionSprite.hpp
        src/render_engine/sdl2/shader/GLSLProgram.cpp
        src/render_engine/sdl2/shader/GLSLProgram.hpp
        src/render_engine/sdl2/geometry/Vertex.hpp
        src/render_engine/sdl2/io/IOManager.cpp
        src/render_engine/sdl2/io/IOManager.hpp
        src/render_engine/sdl2/lib/picopng.cpp
        src/render_engine/sdl2/lib/picoPNG.hpp
        src/render_engine/sdl2/shader/GLTexture.hpp
        src/render_engine/sdl2/shader/ImageLoader.cpp
        src/render_engine/sdl2/shader/ImageLoader.hpp)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})
target_link_libraries(SIRenderEngine ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES} SIRuntime)
add_dependencies(SIRenderEngine SIRuntime)