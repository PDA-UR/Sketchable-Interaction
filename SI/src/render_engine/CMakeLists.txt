cmake_minimum_required(VERSION 3.13)
project(SIRenderEngine)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

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
        src/render_engine/sdl2/geometry/GLTexture.hpp
        src/render_engine/sdl2/io/ImageLoader.cpp
        src/render_engine/sdl2/io/ImageLoader.hpp
        src/render_engine/sdl2/io/ResourceManager.cpp
        src/render_engine/sdl2/io/ResourceManager.hpp
        src/render_engine/sdl2/geometry/TextureCache.cpp
        src/render_engine/sdl2/geometry/TextureCache.hpp
        src/render_engine/debug/SiRenPrint.cpp
        src/render_engine/debug/SiRenPrint.hpp
        src/render_engine/sdl2/display/Window.cpp
        src/render_engine/sdl2/display/Window.hpp
        src/render_engine/sdl2/display/Camera2D.cpp
        src/render_engine/sdl2/display/Camera2D.hpp
        src/render_engine/sdl2/geometry/SpriteBatch.cpp
        src/render_engine/sdl2/geometry/SpriteBatch.hpp
        src/render_engine/sdl2/geometry/Glyph.hpp)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})
target_link_libraries(SIRenderEngine glm ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES} SIRuntime)
add_dependencies(SIRenderEngine SIRuntime)