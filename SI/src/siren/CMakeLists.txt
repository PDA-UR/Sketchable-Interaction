cmake_minimum_required(VERSION 3.13)
project(SIEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

set(SIRENDERENGINESrc
        SI/src/siren/RenderEngineSdl2.cpp
        SI/src/siren/RenderEngineSdl2.hpp
        SI/src/siren/display/Window.c
        SI/src/siren/display/Window.h
        SI/src/siren/display/Camera2D.cpp
        SI/src/siren/display/Camera2D.hpp
        SI/src/siren/shader/GLSLProgram.cpp
        SI/src/siren/shader/GLSLProgram.hpp
        SI/src/siren/lib/picopng.cpp
        SI/src/siren/lib/picoPNG.hpp
        SI/src/siren/geometry/SpriteBatch.cpp
        SI/src/siren/geometry/SpriteBatch.hpp
        SI/src/siren/geometry/Vertex.hpp
        SI/src/siren/geometry/TextureCache.cpp
        SI/src/siren/geometry/TextureCache.hpp
        SI/src/siren/geometry/Glyph.hpp
        SI/src/siren/geometry/GLTexture.hpp
        SI/src/siren/io/ImageLoader.cpp
        SI/src/siren/io/ImageLoader.hpp
        SI/src/siren/io/IOManager.cpp
        SI/src/siren/io/IOManager.hpp
        SI/src/siren/io/ResourceManager.cpp
        SI/src/siren/io/ResourceManager.hpp
        SI/src/siren/display/Timing.hpp
        SI/src/siren/display/Timing.cpp
        SI/src/siren/geometry/Tesselator.cpp
        SI/src/siren/geometry/Tesselator.hpp)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})
target_link_libraries(SIRenderEngine glm ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES} SIGeneralRuntime)
add_dependencies(SIRenderEngine PySI SIGeneralRuntime)