cmake_minimum_required(VERSION 3.17)

project(StaticScript CXX)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DEBUG_MODE true)
endif ()

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(AddANTLR)

include(AddLLVM)

include(AddDoxygen)

configure_file(include/Config/Config.h.in ${PROJECT_BINARY_DIR}/include/Config/Config.h)

include_directories(${PROJECT_BINARY_DIR}/include)

include_directories(include)

add_subdirectory(lib)

add_subdirectory(src)

add_executable(
        staticscript
        $<TARGET_OBJECTS:parser>
        $<TARGET_OBJECTS:entity>
        $<TARGET_OBJECTS:ast>
        $<TARGET_OBJECTS:sema>
        $<TARGET_OBJECTS:codegen>
        $<TARGET_OBJECTS:optimization>
        $<TARGET_OBJECTS:driver>
)

add_dependencies(staticscript antlr4_static)

target_link_libraries(staticscript PRIVATE antlr4_static ${llvm_libs})