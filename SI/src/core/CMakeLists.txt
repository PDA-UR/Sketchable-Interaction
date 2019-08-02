cmake_minimum_required(VERSION 3.13)
project(SIEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PythonLibs 3 REQUIRED)
find_package(Python3 REQUIRED)
find_package(Boost 1.69.0 COMPONENTS system python3.7 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

set(xlib TRUE)
if(xlib)
    add_definitions(-Dxlib)
endif()

set(PySISrc
        src/core/pysi_core/Plugin.cpp
        src/core/pysi_core/Plugin.hpp)

# SI PYTHON3 MODULE
add_library(PySI MODULE ${PySISrc})
target_link_libraries(PySI ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# SI RUNTIME LIBRARY

set(SIRUNTIMESrc
        src/core/Core.cpp
        src/core/Core.hpp
        src/SIEngine.cpp
        src/SIEngine.hpp
        src/core/pysi_core/Plugin.cpp
        src/core/pysi_core/Plugin.hpp
        src/core/pysi_core/Scripting.cpp
        src/core/pysi_core/Scripting.hpp
        src/core/runtime_core/region/Region.cpp
        src/core/runtime_core/region/Region.hpp
        src/core/debug/Print.hpp
        src/core/pysi_core/PluginCollector.cpp
        src/core/pysi_core/PluginCollector.hpp
        src/core/runtime_core/collision/CollisionMask.cpp
        src/core/runtime_core/collision/CollisionMask.hpp
        src/core/runtime_core/collision/CollisionManager.cpp
        src/core/runtime_core/collision/CollisionManager.hpp
        src/core/context/Context.cpp
        src/core/context/Context.hpp
        src/core/context/managers/LayerManager.cpp
        src/core/context/managers/LayerManager.hpp
        src/core/runtime_core/region/Layer.cpp
        src/core/runtime_core/region/Layer.hpp
        src/core/context/managers/PersistenceManager.cpp
        src/core/context/managers/PersistenceManager.hpp
        src/core/context/managers/PluginManager.cpp
        src/core/context/managers/PluginManager.hpp
        src/core/context/managers/RegionManager.cpp
        src/core/context/managers/RegionManager.hpp
        src/core/math/si_math.hpp
        src/core/render_engine_core/IRenderEngine.hpp
        src/core/concurrency/StoppableTask.cpp
        src/core/concurrency/StoppableTask.hpp)

add_library(SIRuntime SHARED ${SIRUNTIMESrc})

if (xlib)
    target_link_libraries(SIRuntime ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} -lX11)
else()
    target_link_libraries(SIRuntime ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
endif()

add_dependencies(SIRuntime PySI)