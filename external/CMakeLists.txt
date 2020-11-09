set(EXTERNAL_BITCODE ss_string.bc)

add_custom_command(
        OUTPUT ${EXTERNAL_BITCODE}
        COMMAND clang -c -emit-llvm ${PROJECT_SOURCE_DIR}/external/ss_string.c -std=c99 -Og -o ss_string.bc
)

add_custom_target(
        external_bitcode
        DEPENDS ${EXTERNAL_BITCODE}
)
