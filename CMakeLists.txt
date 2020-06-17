cmake_minimum_required(VERSION 3.16)

project(StaticScript)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/antlr4)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
    set(ANTLR4_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.8-complete.jar)
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
    set(ANTLR4_JAR_LOCATION /usr/local/opt/antlr/antlr-4.8-complete.jar)
endif ()

find_package(antlr4-runtime REQUIRED)

find_package(antlr4-generator REQUIRED)

antlr4_generate(
        StaticScriptLexer
        ${PROJECT_SOURCE_DIR}/grammar/StaticScriptLexer.g4
        LEXER
        FALSE
        FALSE
)

antlr4_generate(
        StaticScriptParser
        ${PROJECT_SOURCE_DIR}/grammar/StaticScriptParser.g4
        PARSER
        FALSE
        TRUE
        ""
        ${ANTLR4_TOKEN_FILES_StaticScriptLexer}
        ${ANTLR4_TOKEN_DIRECTORY_StaticScriptLexer}
)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${ANTLR4_INCLUDE_DIR}
        ${ANTLR4_INCLUDE_DIR_StaticScriptLexer}
        ${ANTLR4_INCLUDE_DIR_StaticScriptParser}
)

aux_source_directory(${PROJECT_SOURCE_DIR}/src STATICSCRIPT_SRC)

add_executable(
        StaticScript
        ${STATICSCRIPT_SRC}
        ${ANTLR4_SRC_FILES_StaticScriptLexer}
        ${ANTLR4_SRC_FILES_StaticScriptParser}
)

add_dependencies(StaticScript antlr4_static)

target_link_libraries(StaticScript PRIVATE antlr4_static)