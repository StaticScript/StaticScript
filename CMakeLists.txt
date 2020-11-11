cmake_minimum_required(VERSION 3.16)

project(StaticScript CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

configure_file(include/config.h.in ${PROJECT_BINARY_DIR}/include/config.h)

include(AddANTLR)

include(AddLLVM)

include(AddDoxygen)

include_directories(${PROJECT_BINARY_DIR}/include)

include_directories(include)

add_subdirectory(lib)

add_subdirectory(builtin)