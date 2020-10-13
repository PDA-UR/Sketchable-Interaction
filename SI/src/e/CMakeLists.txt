

cmake_minimum_required(VERSION 3.16)
project(SIEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(eSRC
    SI/src/e/E.cpp
    SI/src/e/E.hpp
    SI/lib/pugixml-1.10/src/pugiconfig.hpp
    SI/lib/pugixml-1.10/src/pugixml.cpp
    SI/lib/pugixml-1.10/src/pugixml.hpp
    )

add_executable(egen ${eSRC})
