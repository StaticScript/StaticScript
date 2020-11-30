set(BUILTIN_LIB_SOURCES ss_error.c ss_string.c ss_array.c ss_io.c)
set(BUILTIN_LIB_BITCODES ss_error.bc ss_string.bc ss_array.bc ss_io.bc)

add_custom_command(
        OUTPUT ${BUILTIN_LIB_BITCODES}
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/lib/ss_error.c -std=c99 -O2 -o ss_error.bc
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/lib/ss_string.c -std=c99 -O2 -o ss_string.bc
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/lib/ss_array.c -std=c99 -O2 -o ss_array.bc
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/lib/ss_io.c -std=c99 -O2 -o ss_io.bc
        COMMAND sudo mkdir -p /usr/local/staticscript/lib && sudo cp ss_*.bc /usr/local/staticscript/lib
        DEPENDS ${BUILTIN_LIB_SOURCES}
)

add_custom_target(
        lib
        DEPENDS ${BUILTIN_LIB_BITCODES}
)