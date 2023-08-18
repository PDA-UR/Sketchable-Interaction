cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Widgets REQUIRED)

set(SIRENDERENGINESrc
        SI/src/siren/RenderEngineQt5.cpp
        SI/src/siren/RenderEngineQt5.hpp
        SI/src/siren/window/MainWindow.cpp
        SI/src/siren/window/MainWindow.hpp
        SI/src/siren/region/RegionRepresentation.cpp
        SI/src/siren/region/RegionRepresentation.hpp
        SI/src/siren/region/PartialRegionRepresentation.cpp
        SI/src/siren/region/PartialRegionRepresentation.hpp
        SI/src/siren/timing/Timing.cpp
        SI/src/siren/timing/Timing.hpp)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

add_library(SIRenderEngine SHARED ${SIRENDERENGINESrc})

target_precompile_headers(SIRenderEngine
        PUBLIC
        PRIVATE
        <unordered_map>
        <vector>
        <QApplication>
        <QMainWindow>
        <QObject>
        <QQmlEngine>
        <QQmlContext>
#        <QDesktopWidget>
        <QScreen>
        <QPaintEvent>
        <QPainter>
        SI/include/SI/SI.hpp
        )

set_target_properties(SIRenderEngine PROPERTIES AUTOMOC TRUE)
target_link_libraries(SIRenderEngine SIGeneralRuntime Qt6::QuickWidgets Qt6::Quick Qt6::Core Qt6::Widgets Qt6::Qml -ltbb)
add_dependencies(SIRenderEngine PySI SIGeneralRuntime)