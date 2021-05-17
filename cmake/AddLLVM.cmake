if (CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
    list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/llvm/lib/cmake/llvm)
endif ()

find_package(LLVM 12 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader linker native codegen nativecodegen Passes ObjCARCOpts)
