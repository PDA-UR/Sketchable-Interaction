cmake_minimum_required(VERSION 3.13)
project(SIEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PythonLibs 3 REQUIRED)
find_package(Python3 REQUIRED)
find_package(Boost 1.69.0 COMPONENTS system python3.7 REQUIRED)
find_package(glm REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

set(PATH_LIB ${CMAKE_CURRENT_SOURCE_DIR}/SI/lib/libTUIO2.a)
set(PATH_INCLUDE_TUIO2_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/SI/lib/tuio_headers/TUIO2)
set(PATH_INCLUDE_OSC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/SI/lib/tuio_headers/oscpack)

set(THREADS_PREFER_PTHREAD_FLAG ON) # linux only
find_package(Threads REQUIRED) # linux only

include_directories(${PATH_INCLUDE_TUIO2_HEADERS} ${PATH_INCLUDE_OSC_HEADERS})

#set(xlib TRUE)
set(xlib FALSE)
if(xlib)
    add_definitions(-Dxlib)
endif()

set(PySISrc
        SI/src/pysi/SuperEffect.cpp
        SI/src/pysi/SuperEffect.hpp)

# SI PYTHON3 MODULE
add_library(PySI MODULE ${PySISrc})
target_link_libraries(PySI ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# SI RUNTIME LIBRARY

set(SIGRunSRC
        SI/src/SIGRun.cpp
        SI/src/SIGRun.hpp
        SI/src/pysi/SuperEffect.cpp
        SI/src/pysi/SuperEffect.hpp
        SI/src/sigrun/Core.cpp
        SI/src/sigrun/Core.hpp
        SI/src/debug/Print.cpp
        SI/src/debug/Print.hpp
        SI/src/sigrun/plugin/PluginCollector.cpp
        SI/src/sigrun/plugin/PluginCollector.hpp
        SI/src/sigrun/plugin/Scripting.cpp
        SI/src/sigrun/plugin/Scripting.hpp
        SI/src/sigrun/log/Log.cpp
        SI/src/sigrun/log/Log.hpp
        SI/src/sigrun/plugin/PythonInvoker.cpp
        SI/src/sigrun/plugin/PythonInvoker.hpp
        SI/src/sigrun/SIObject.hpp
        SI/src/sigrun/region/RegionTransform.cpp
        SI/src/sigrun/region/RegionTransform.hpp
        SI/src/sigrun/region/RegionMask.cpp
        SI/src/sigrun/region/RegionMask.hpp
        SI/src/sigrun/context/Capability.cpp
        SI/src/sigrun/context/Capability.hpp
        SI/src/sigrun/region/Region.cpp
        SI/src/sigrun/region/Region.hpp
        SI/src/error/Error.hpp)

add_library(SIGeneralRuntime SHARED ${SIGRunSRC})

if (xlib)
    target_link_libraries(SIGeneralRuntime ${PATH_LIB} glm ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} -lX11)
else()
    target_link_libraries(SIGeneralRuntime ${PATH_LIB} glm ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
endif()


if (BUILD_DOCS)
    find_package(Doxygen)

    message("Documentation Generation enabled.")

    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN SI/docs/doxyfiles/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/doxyfiles/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(SIDocs ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si
                COMMENT "Generating API documentation with Doxygen")

        add_custom_target(SIDocsPDF
                COMMENT "Generating API documentation with Doxygen as PDF"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/latex
                COMMAND make
                COMMAND mv refman.pdf ../SIGRunDoc.pdf)

        message("Doxygen build installed")

        install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target doc)")

    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)

    add_dependencies(SIDocsPDF SIDocs)
    add_dependencies(SIGeneralRuntime PySI SIDocsPDF)

else (BUILD_DOCS)
    message("Documentation Generation disabled.")
    add_dependencies(SIGeneralRuntime PySI)
endif(BUILD_DOCS)
