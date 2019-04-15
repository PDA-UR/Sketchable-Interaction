cmake_minimum_required(VERSION 3.0)
project(silang C)

set(CMAKE_C_STANDARD 11)


# Create target for the parser
ADD_CUSTOM_TARGET(parser echo "Creating parser.c")

# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_COMMAND(
        COMMAND lex
        ARGS -o ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
        TARGET parser
        OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
        COMMAND yacc -d
        ARGS -y ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
        -o ${CMAKE_CURRENT_BINARY_DIR}/parser.c
        TARGET parser
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
        OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/parser.c)

# Add parser.c to the list of sources
SET(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/parser.c)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/parser.c GENERATED)

# Include binary directory to include lexer.c in parser.c
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/)



add_executable(silang lexer.c parser.c main.c ast.c ast.h error.c error.h)
add_dependencies(silang parser)
