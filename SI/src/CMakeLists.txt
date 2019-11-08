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
        SI/src/sigrun/MetaType.hpp)

add_library(SIGeneralRuntime SHARED ${SIGRunSRC})

if (xlib)
    target_link_libraries(SIGeneralRuntime ${PATH_LIB} glm ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} -lX11)
else()
    target_link_libraries(SIGeneralRuntime ${PATH_LIB} glm ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
endif()

#add_dependencies(SIGeneralRuntime PySI SIDocsPDF)
add_dependencies(SIGeneralRuntime PySI)
