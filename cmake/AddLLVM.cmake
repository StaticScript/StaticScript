list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/llvm/lib/cmake/llvm)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader linker orcjit nativecodegen)
