cmake_minimum_required(VERSION 3.1)
project(core)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(SRC
        src/si.cpp
        include/si.h
        include/debug.h
        src/engine.cpp
        include/engine.h
        src/region.cpp
        include/region.h
        src/step.cpp
        include/step.h
        src/effect.cpp
        include/effect.h
        src/canvas_region.cpp
        include/canvas_region.h
        src/mouse_region.cpp
        include/mouse_region.h
        include/util.h)

SET(SRC_TEST_APPLICATION
        src/main.cpp)

SET(SRC_GTEST_RUNNER
        test/src/unit_tests_run.cpp
        test/src/si_test.cpp
        test/include/si_test.h
        test/include/cmd_args.h
        test/src/si_engine_test.cpp
        test/include/si_engine_test.h
        test/src/si_util_test.cpp
        test/include/si_util_test.h)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )

    message("Doxygen build installed")

    install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target doc)")


else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# SI CORE LIBRARY
add_library(si-core SHARED ${SRC})
set_target_properties(si-core PROPERTIES AUTOMOC TRUE)
target_link_libraries(si-core Qt5::Widgets)

# SI TESTING APPLICATION
add_executable(si_engine_test ${SRC_TEST_APPLICATION})
target_link_libraries(si_engine_test cmake-build-debug/si-core)

# SI GTEST UNIT TEST RUNNER

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
        ${gmock_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include)

add_subdirectory(../../googletest googletest)

add_executable(unit_tests_run ${SRC_GTEST_RUNNER})
set_target_properties(unit_tests_run PROPERTIES AUTOMOC TRUE)
target_link_libraries(unit_tests_run Qt5::Widgets Qt5::Test cmake-build-debug/si-core gtest gtest_main gmock gmock_main pthread)

