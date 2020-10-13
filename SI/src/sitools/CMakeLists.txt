cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Widgets REQUIRED)

set(sitoolsSRC
        SI/src/sitools/sitools.cpp
        SI/src/sitools/sitools.hpp)

add_executable(sitools ${sitoolsSRC})
set_target_properties(sitools PROPERTIES AUTOMOC TRUE)

target_link_libraries(sitools Qt5::Core Qt5::Widgets)