cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# sudo apt install python3.9-dev
# Boost System, Filesystem, Boost.Python compilen mit Python3.9
# dependent on current install of boostpython                 |
#                                                             v
find_package(Boost 1.69.0 EXACT REQUIRED COMPONENTS system python3.9 filesystem REQUIRED)

find_package(Python REQUIRED)
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
find_package(glm REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON) # linux only
find_package(Threads REQUIRED) # linux only

set(CMAKE_PREFIX_PATH "/usr/local/Qt6/lib/cmake")

FIND_PACKAGE(Qt6Widgets REQUIRED)
FIND_PACKAGE(Qt6Qml REQUIRED)
FIND_PACKAGE(Qt6QuickWidgets REQUIRED)
FIND_PACKAGE(Qt6Quick REQUIRED)
FIND_PACKAGE(Qt6Multimedia REQUIRED)
find_package(Qt6Test REQUIRED)
find_package(Qt6Core5Compat REQUIRED)
find_package(Qt6OpenGLWidgets REQUIRED)
find_package(Qt6Quick REQUIRED)
find_package(Qt6QuickControls2 REQUIRED)
find_package(Qt6QmlWorkerScript REQUIRED)

OPTION(DEFINE_TEST "ENABLE TEST MODE" OFF)

IF(DEFINE_TEST)
    ADD_DEFINITIONS(-DIS_TEST)
ENDIF(DEFINE_TEST)

set(PySISrc
        SI/src/pysi/PySIEffect.cpp
        SI/src/pysi/PySI.cpp
        SI/src/pysi/PySIStartup.cpp)

# SI PYTHON3 MODULE
add_library(PySI MODULE ${PySISrc})
target_link_libraries(PySI
        ${PYTHON_LIBRARIES}
        ${Boost_LIBRARIES}
        Qt6::Widgets
        Qt6::Multimedia
        -ltbb)

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
        SI/src/pysi/pickling/PickleSuits.hpp
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
        SI/src/sigrun/util/Dollar1GestureRecognizer.hpp
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
        SI/src/sigrun/plugin/PythonGlobalInterpreterLockGuard.hpp
        SI/src/sigrun/log/Log.cpp
        SI/src/sigrun/log/Log.hpp
        SI/src/sigrun/log/CrashDump.cpp
        SI/src/sigrun/log/CrashDump.hpp
        SI/src/sigrun/region/RegionTransform.cpp
        SI/src/sigrun/region/RegionTransform.hpp
        SI/src/sigrun/region/RegionMask.cpp
        SI/src/sigrun/region/RegionMask.hpp
        SI/src/sigrun/region/Region.cpp
        SI/src/sigrun/region/Region.hpp
        SI/src/sigrun/SIMessages.hpp
        SI/src/sigrun/context/managers/CollisionManager.cpp
        SI/src/sigrun/context/managers/CollisionManager.hpp
        SI/src/sigrun/context/Context.cpp
        SI/src/sigrun/context/Context.hpp
        SI/src/sigrun/context/managers/RegionManager.cpp
        SI/src/sigrun/context/managers/RegionManager.hpp
        SI/src/sigrun/region/RegionResampler.cpp
        SI/src/sigrun/region/RegionResampler.hpp
        SI/src/sigrun/rendering/IRenderEngine.hpp
        SI/src/sigrun/network/IPhysicalEnvironment.hpp
        SI/src/sigrun/network/TangibleObjectMessage.cpp
        SI/src/sigrun/network/TangibleObjectMessage.hpp
        SI/src/sigrun/rendering/qml/items/PlotItem.cpp
        SI/src/sigrun/rendering/qml/items/PlotItem.hpp
        SI/src/sigrun/context/managers/MultiMouseKeyboardManager.cpp
        SI/src/sigrun/context/managers/MultiMouseKeyboardManager.hpp
#        SI/src/sigrun/rendering/qml/items/VideoItem.cpp
#        SI/src/sigrun/rendering/qml/items/VideoItem.hpp
        SI/src/sigrun/context/managers/LinkingManager.cpp
        SI/src/sigrun/context/managers/LinkingManager.hpp
        SI/src/sigrun/context/managers/helpers/linking/Link.cpp
        SI/src/sigrun/context/managers/helpers/linking/Link.hpp
        SI/src/sigrun/context/managers/helpers/linking/LinkCandidate.hpp
        SI/src/sigrun/context/managers/InputManager.cpp
        SI/src/sigrun/context/managers/InputManager.hpp
        SI/src/sigrun/context/managers/helpers/input/ExternalObject.cpp
        SI/src/sigrun/context/managers/helpers/input/ExternalObject.hpp
        SI/src/sigrun/context/managers/ExternalApplicationManager.cpp
        SI/src/sigrun/context/managers/ExternalApplicationManager.hpp
        SI/src/sigrun/parallel/JobSystem.hpp
        SI/src/sigrun/parallel/Job.cpp
        SI/src/sigrun/parallel/Job.hpp
        SI/src/sigrun/parallel/helpers/ThreadSafeRingBuffer.hpp
        SI/src/sigrun/util/Dollar1GestureRecognizer.cpp
        SI/src/sigrun/util/Dollar1GestureRecognizer.hpp
        SI/src/sigrun/context/managers/TangibleManager.cpp
        SI/src/sigrun/context/managers/TangibleManager.hpp
        SI/src/sigrun/plugin/PythonInvoker.cpp
        SI/src/sigrun/context/spatial_grid/SpatialHashGrid.cpp
        SI/src/sigrun/context/spatial_grid/SpatialHashGrid.hpp
        SI/src/e/E.cpp
        SI/src/e/E.hpp

        SI/lib/pugixml-1.10/src/pugiconfig.hpp
        SI/lib/pugixml-1.10/src/pugixml.cpp
        SI/lib/pugixml-1.10/src/pugixml.hpp)

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
        -luuid
        ${PYTHON_LIBRARIES}
        ${Boost_LIBRARIES}
        Qt6::QuickWidgets
        Qt6::Core5Compat
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Core
        Qt6::Test
        Qt6::Widgets
        Qt6::Qml
        Qt6::Gui
        Qt6::Multimedia
        Qt6::QmlWorkerScript
        Qt6::OpenGLWidgets
        -ltbb
        -ldl
        -lbacktrace
        -linput)

# SI plugin generator src
set(SIGenSRC
    SI/src/sipluggen/sipluggen.cpp
    SI/src/sipluggen/templates.h)

add_executable(sipluggen ${SIGenSRC})
target_link_libraries(sipluggen ${Boost_LIBRARIES})

if (BUILD_DOCS)
    message("Documentation Generation enabled.")

    find_package(Doxygen)

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
                COMMAND mv refman.pdf ../SIGRunDoc.pdf
                COMMAND rm -rf ../html/docs
                COMMAND mkdir ../docs
                COMMAND mv ../html/* ../docs
                COMMAND mv ../docs ../html
                COMMAND touch ../html/sigrun.html
                COMMAND echo '<meta http-equiv="REFRESH" content="0;URL=docs/index.html">' >> ../html/sigrun.html )

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
                COMMAND mv refman.pdf ../PySIDoc.pdf
                COMMAND rm -rf ../html/docs
                COMMAND mkdir ../docs
                COMMAND mv ../html/* ../docs
                COMMAND mv ../docs ../html
                COMMAND touch ../html/pysi.html
                COMMAND echo '<meta http-equiv="REFRESH" content="0;URL=docs/index.html">' >> ../html/pysi.html )

        message("Doxygen build installed")

        install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target doc)")

    else (DOXYGEN_FOUND)
        message("Doxygen needs to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)

    add_custom_target(APIExamplesPDF
        COMMAND pandoc ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/api_examples/api_examples.md -o ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/api_examples/api_examples.pdf
        COMMAND pandoc ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/api_examples/api_examples.md -o ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/api_examples/api_examples.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/api_examples
    )

    add_dependencies(SIGRunDocsPDF SIGRunDocs)
    add_dependencies(PySIDocsPDF PySIDocs)
    add_dependencies(SIGeneralRuntime PySI SIGRunDocsPDF PySIDocsPDF APIExamplesPDF)

else (BUILD_DOCS)
    message("Documentation Generation disabled.")
    add_dependencies(SIGeneralRuntime PySI)
endif(BUILD_DOCS)
