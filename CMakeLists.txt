cmake_minimum_required(VERSION 3.13)
project(SI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O2")

#SIEngine
include(SI/src/CMakeLists.txt)

# SIExamples
include(SI/examples/app/CMakeLists.txt)

# SITests
include(SI/tests/CMakeLists.txt)

#find_package(Doxygen)

#if (DOXYGEN_FOUND)
#    # set input and output files
#    set(DOXYGEN_IN SI/docs/doxyfiles/Doxyfile.in)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/SI/docs/doxyfiles/Doxyfile)
#
#    # request to configure the file
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#    message("Doxygen build started")
#
#    # note the option ALL which allows to build the docs together with the application
#    add_custom_target(SIDocs ALL
#            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si
#            COMMENT "Generating API documentation with Doxygen")
#
#    add_custom_target(SIDocsPDF
#            COMMENT "Generating API documentation with Doxygen as PDF"
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SI-Doc/si/latex
#            COMMAND make
#            COMMAND mv refman.pdf ../SIGRunDoc.pdf)
#
#    message("Doxygen build installed")
#
#    install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target doc)")
#
#
#else (DOXYGEN_FOUND)
#    message("Doxygen need to be installed to generate the doxygen documentation")
#endif (DOXYGEN_FOUND)
#
#add_dependencies(SIDocsPDF SIDocs)


