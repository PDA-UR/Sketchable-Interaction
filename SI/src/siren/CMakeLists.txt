cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(SDL2 REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(glm REQUIRED)
find_package(Qt5Widgets REQUIRED)

#include_directories(${SDL2_INCLUDE_DIR})
#include_directories(${GLEW_INCLUDE_DIRS})
#include_directories(${OPENGL_INCLUDE_DIRS})

set(SIRENDERENGINESrc
#        SI/src/siren/RenderEngineSdl2.cpp
#        SI/src/siren/RenderEngineSdl2.hpp
#        SI/src/siren/display/Window.cpp
#        SI/src/siren/display/Window.hpp
#        SI/src/siren/display/Camera2D.cpp
#        SI/src/siren/display/Camera2D.hpp
#        SI/src/siren/shader/GLSLProgram.cpp
#        SI/src/siren/shader/GLSLProgram.hpp
#        SI/src/siren/lib/picopng.cpp
#        SI/src/siren/lib/picoPNG.hpp
#        SI/src/siren/geometry/SpriteBatch.cpp
#        SI/src/siren/geometry/SpriteBatch.hpp
#        SI/src/siren/geometry/Vertex.hpp
#        SI/src/siren/geometry/TextureCache.cpp
#        SI/src/siren/geometry/TextureCache.hpp
#        SI/src/siren/geometry/Glyph.hpp
#        SI/src/siren/geometry/GLTexture.hpp
#        SI/src/siren/io/ImageLoader.cpp
#        SI/src/siren/io/ImageLoader.hpp
#        SI/src/siren/io/IOManager.cpp
#        SI/src/siren/io/IOManager.hpp
#        SI/src/siren/io/ResourceManager.cpp
#        SI/src/siren/io/ResourceManager.hpp
#        SI/src/siren/display/Timing.hpp
#        SI/src/siren/geometry/Tessellator.cpp
#        SI/src/siren/geometry/Tessellator.hpp
#        SI/src/siren/geometry/TessellationPatch.cpp
#        SI/src/siren/geometry/TessellationPatch.hpp
#        SI/src/siren/region_representation/RegionRepresentation.hpp
        SI/src/siren/RenderEngineQt5.cpp
        SI/src/siren/RenderEngineQt5.hpp
        SI/src/siren/window/MainWindow.cpp
        SI/src/siren/window/MainWindow.hpp
        SI/src/siren/background/UpdateWorker.cpp)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})

target_precompile_headers(SIRenderEngine
        PUBLIC
        PRIVATE
        <unordered_map>
        <vector>
        <boost/python/suite/indexing/vector_indexing_suite.hpp>
        <boost/python/suite/indexing/map_indexing_suite.hpp>
        <boost/python.hpp>
        <QApplication>
        <QMainWindow>
        <QObject>
        <QMetaMethod>
        <QPaintEvent>
        <QPainter>
        SI/src/siren/background/UpdateWorker.hpp
        SI/src/siren/region/RegionRepresentation.hpp
        SI/include/SI/SI.hpp
        )

set_target_properties(SIRenderEngine PROPERTIES AUTOMOC TRUE)
#target_link_libraries(SIRenderEngine ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES} SIGeneralRuntime Qt5::Widgets)
target_link_libraries(SIRenderEngine SIGeneralRuntime Qt5::QuickWidgets Qt5::Quick Qt5::Core Qt5::Widgets Qt5::Qml)
add_dependencies(SIRenderEngine PySI SIGeneralRuntime)