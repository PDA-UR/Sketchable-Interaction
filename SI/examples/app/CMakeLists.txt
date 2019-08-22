cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SIExample)

set(SITestAppSrc
        examples/app/src/main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(si_test_application ${SITestAppSrc})
#set_target_properties(si_test_application PROPERTIES AUTOMOC TRUE)
#target_link_libraries(si_test_application SI Qt5::Widgets)
target_link_libraries(si_test_application ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} SIRenderEngine SIRuntime)
add_dependencies(si_test_application PySI SIRuntime SIRenderEngine)