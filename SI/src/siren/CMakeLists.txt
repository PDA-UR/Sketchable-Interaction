cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Widgets REQUIRED)

set(SIRENDERENGINESrc
        SI/src/siren/RenderEngineQt5.cpp
        SI/src/siren/RenderEngineQt5.hpp
        SI/src/siren/window/MainWindow.cpp
        SI/src/siren/window/MainWindow.hpp
        SI/src/siren/background/UpdateWorker.cpp
        SI/src/siren/region/RegionRepresentation.cpp)

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
        <QQmlEngine>
        <QQmlContext>
        <QPaintEvent>
        <QPainter>
        SI/src/siren/background/UpdateWorker.hpp
        SI/src/siren/region/RegionRepresentation.hpp
        SI/include/SI/SI.hpp
        )

set_target_properties(SIRenderEngine PROPERTIES AUTOMOC TRUE)
target_link_libraries(SIRenderEngine SIGeneralRuntime Qt5::QuickWidgets Qt5::Quick Qt5::Core Qt5::Widgets Qt5::Qml)
add_dependencies(SIRenderEngine PySI SIGeneralRuntime)