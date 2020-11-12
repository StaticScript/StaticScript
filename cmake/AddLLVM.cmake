if (CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
    find_package(LLVM 10 REQUIRED CONFIG)
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
    list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/llvm/lib/cmake/llvm)
    find_package(LLVM 11 REQUIRED CONFIG)
endif ()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader linker nativecodegen)
