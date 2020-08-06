cmake_minimum_required(VERSION 3.16)

project(StaticScript CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(AddANTLR)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(lib)