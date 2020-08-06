add_subdirectory(AST)
add_subdirectory(Driver)

add_executable(
        staticscript
        $<TARGET_OBJECTS:parser>
        $<TARGET_OBJECTS:ast>
        $<TARGET_OBJECTS:driver>
)

add_dependencies(staticscript antlr4_static)

target_link_libraries(staticscript PRIVATE antlr4_static)
