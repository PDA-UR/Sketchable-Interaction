cmake_minimum_required(VERSION 3.13)
project(c_ogl_si_test C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${SDL2_INCLUDE_DIRS})

add_executable(c_ogl_si_test src/main.c src/engine.c src/engine.h src/rect.c src/rect.h src/shader.c src/shader.h src/vertex.h)
target_link_libraries(c_ogl_si_test ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})