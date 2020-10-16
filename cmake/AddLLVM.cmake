list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/llvm/lib/cmake/llvm)

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS})

link_directories(${LLVM_LIBRARY_DIRS})

llvm_map_components_to_libnames(llvm_libs support core orcjit nativecodegen)
