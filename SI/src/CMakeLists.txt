cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(Python_ADDITIONAL_VERSIONS 3.7)
find_package(PythonLibs 3.7 REQUIRED)
find_package(Python3 REQUIRED)
find_package(Boost 1.69.0 COMPONENTS system python3.7 filesystem REQUIRED)
find_package(glm REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

set(PATH_LIB ${CMAKE_CURRENT_SOURCE_DIR}/SI/lib/libTUIO2.a)
set(PATH_INCLUDE_TUIO2_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/SI/lib/tuio_headers/TUIO2)
set(PATH_INCLUDE_OSC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/SI/lib/tuio_headers/oscpack)

set(THREADS_PREFER_PTHREAD_FLAG ON) # linux only
find_package(Threads REQUIRED) # linux only

find_package(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Qml REQUIRED)
FIND_PACKAGE(Qt5QuickWidgets REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)

include_directories(${PATH_INCLUDE_TUIO2_HEADERS} ${PATH_INCLUDE_OSC_HEADERS})

set(PySISrc
        SI/src/pysi/PySIEffect.cpp
        SI/src/pysi/PySI.cpp
        SI/src/pysi/PySIStartup.cpp)

# SI PYTHON3 MODULE
add_library(PySI MODULE ${PySISrc})
target_link_libraries(PySI
        ${PYTHON_LIBRARIES} 
        ${Boost_LIBRARIES}
        Qt5::Widgets -ltbb)

target_precompile_headers(PySI
        PUBLIC
        PRIVATE
        <unordered_map>
        <vector>
        <algorithm>
        <numeric>
        <execution>
        <QVariant>
        <map>
        SI/src/pysi/stl_container_exposure/VectorExposure.hpp
        SI/src/pysi/stl_container_exposure/MapExposure.hpp
        <boost/python/suite/indexing/vector_indexing_suite.hpp>
        <boost/python/suite/indexing/map_indexing_suite.hpp>
        <boost/python.hpp>
        SI/src/sigrun/util/Util.hpp
        <glm/glm.hpp>
        SI/src/pysi/PySIEffect.hpp
        SI/src/pysi/PySIStartup.hpp
        SI/src/sigrun/plugin/PythonInvoker.hpp
        SI/src/sigrun/SITypes.hpp
        SI/src/sigrun/SIConstants.hpp
        )

# SI RUNTIME LIBRARY
set(SIGRunSRC
        SI/src/SIGRun.cpp
        SI/src/SIGRun.hpp
        SI/src/pysi/PySIEffect.cpp
        SI/src/pysi/PySI.cpp
        SI/src/pysi/PySIStartup.cpp
        SI/src/sigrun/Core.cpp
        SI/src/sigrun/Core.hpp
        SI/src/debug/Print.cpp
        SI/src/sigrun/plugin/PluginCollector.cpp
        SI/src/sigrun/plugin/PluginCollector.hpp
        SI/src/sigrun/plugin/Scripting.cpp
        SI/src/sigrun/plugin/Scripting.hpp
        SI/src/sigrun/log/Log.cpp
        SI/src/sigrun/log/Log.hpp
        SI/src/sigrun/region/RegionTransform.cpp
        SI/src/sigrun/region/RegionTransform.hpp
        SI/src/sigrun/region/RegionMask.cpp
        SI/src/sigrun/region/RegionMask.hpp
        SI/src/sigrun/region/Region.cpp
        SI/src/sigrun/region/Region.hpp
        SI/src/error/Error.hpp
        SI/src/sigrun/context/managers/CollisionManager.cpp
        SI/src/sigrun/context/managers/CollisionManager.hpp
        SI/src/sigrun/context/Context.cpp
        SI/src/sigrun/context/Context.hpp
        SI/src/sigrun/context/managers/RegionManager.cpp
        SI/src/sigrun/context/managers/RegionManager.hpp
        SI/src/sigrun/region/RegionResampler.cpp
        SI/src/sigrun/region/RegionResampler.hpp
        SI/src/sigrun/rendering/IRenderEngine.hpp
        SI/src/sigrun/context/managers/LinkingManager.cpp
        SI/src/sigrun/context/managers/LinkingManager.hpp
        SI/src/sigrun/context/managers/helpers/linking/Link.cpp
        SI/src/sigrun/context/managers/helpers/linking/Link.hpp
        SI/src/sigrun/context/managers/helpers/linking/LinkCandidate.hpp
        SI/src/sigrun/context/managers/InputManager.cpp
        SI/src/sigrun/context/managers/InputManager.hpp
        SI/src/sigrun/context/managers/helpers/input/ExternalObject.cpp
        SI/src/sigrun/context/managers/helpers/input/ExternalObject.hpp
        SI/src/sigrun/context/managers/helpers/input/FileSystem.cpp
        SI/src/sigrun/context/managers/helpers/input/FileSystem.hpp
        SI/src/sigrun/context/managers/ExternalApplicationManager.cpp
        SI/src/sigrun/context/managers/ExternalApplicationManager.hpp)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

add_library(SIGeneralRuntime SHARED ${SIGRunSRC})

target_precompile_headers(SIGeneralRuntime
        PUBLIC
        PRIVATE
        <unordered_map>
        <vector>
        <algorithm>
        <numeric>
        <execution>
        <string>
        <any>
        <queue>
        <memory>
        <map>
        <boost/python/suite/indexing/vector_indexing_suite.hpp>
        <boost/python/suite/indexing/map_indexing_suite.hpp>
        <boost/python.hpp>
        <boost/filesystem.hpp>
        <QApplication>
        <QInputEvent>
        <QMetaMethod>
        <QObject>
        <QString>
        <QEvent>
        <QProcess>
        <QMainWindow>
        <QWindow>
        <QKeyEvent>
        <QMouseEvent>
        SI/src/sigrun/util/Benchmark.hpp
        SI/src/sigrun/util/UUID.hpp
        SI/src/sigrun/util/RingBuffer.hpp
        SI/src/debug/Print.hpp
        SI/src/sigrun/SIObject.hpp
        SI/src/sigrun/SITypes.hpp
        <glm/glm.hpp>
        SI/src/sigrun/util/Util.hpp
        SI/src/pysi/PySIEffect.hpp
        SI/src/pysi/PySIStartup.hpp
        SI/src/sigrun/plugin/PythonInvoker.hpp
        SI/src/sigrun/SIConstants.hpp
        )

set_target_properties(SIGeneralRuntime PROPERTIES AUTOMOC TRUE)
target_link_libraries(SIGeneralRuntime
        ${PATH_LIB}
        -luuid
        ${PYTHON_LIBRARIES}
        ${Boost_LIBRARIES}
        Qt5::QuickWidgets
        Qt5::Quick
        Qt5::Core
        Qt5::Widgets
        Qt5::Qml
        -ltbb)

if (BUILD_DOCS)
    find_package(Doxygen)

    message("Documentation Generation enabled.")

    if (DOXYGEN_FOUND)
        message("Generating SIGRun Documentation")
        # set input and output files
        set(DOXYGEN_IN SI/docs/doxyfiles/DoxyfileSIGRun.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/doxyfiles/DoxyfileSIGRun)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(SIGRunDocs ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/sigrun
                COMMENT "Generating API documentation with Doxygen")

        add_custom_target(SIGRunDocsPDF
                COMMENT "Generating API documentation with Doxygen as PDF"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/sigrun/latex
                COMMAND make
                COMMAND mv refman.pdf ../SIGRunDoc.pdf)

        message("Doxygen build installed")

        install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target doc)")

        ############################

        message("Generating PySI Documentation")
        # set input and output files
        set(DOXYGEN_IN SI/docs/doxyfiles/DoxyfilePySI.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/doxyfiles/DoxyfilePySI)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(PySIDocs ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/pysi
                COMMENT "Generating API documentation with Doxygen")

        add_custom_target(PySIDocsPDF
                COMMENT "Generating API documentation with Doxygen as PDF"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/pysi/latex
                COMMAND make
                COMMAND mv refman.pdf ../PySIDoc.pdf)

        message("Doxygen build installed")

        install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target doc)")

    else (DOXYGEN_FOUND)
        message("Doxygen needs to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)

    add_dependencies(SIGRunDocsPDF SIGRunDocs)
    add_dependencies(PySIDocsPDF PySIDocs)
    add_dependencies(SIGeneralRuntime PySI SIGRunDocsPDF PySIDocsPDF)

else (BUILD_DOCS)
    message("Documentation Generation disabled.")
    add_dependencies(SIGeneralRuntime PySI)
endif(BUILD_DOCS)
