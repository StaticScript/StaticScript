cmake_minimum_required(VERSION 3.17)

project(StaticScript CXX)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(AddANTLR)

include(AddLLVM)

include(AddDoxygen)

configure_file(include/config.h.in ${PROJECT_BINARY_DIR}/include/config.h)

include_directories(${PROJECT_BINARY_DIR}/include)

include_directories(include)

add_subdirectory(builtin)

add_subdirectory(lib)