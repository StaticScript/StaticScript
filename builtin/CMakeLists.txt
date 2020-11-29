set(BUILTIN_SOURCES ss_error.c ss_string.c ss_array.c ss_io.c)
set(BUILTIN_BITCODES ss_error.bc ss_string.bc ss_array.bc ss_io.bc)

add_custom_command(
        OUTPUT ${BUILTIN_BITCODES}
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/builtin/ss_error.c -std=c99 -O2 -o ss_error.bc
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/builtin/ss_string.c -std=c99 -O2 -o ss_string.bc
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/builtin/ss_array.c -std=c99 -O2 -o ss_array.bc
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/builtin/ss_io.c -std=c99 -O2 -o ss_io.bc
        DEPENDS ${BUILTIN_SOURCES}
)

add_custom_target(
        builtin
        DEPENDS ${BUILTIN_BITCODES}
)