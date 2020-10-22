add_subdirectory(Entity)
add_subdirectory(AST)
add_subdirectory(Sema)
add_subdirectory(CodeGen)
add_subdirectory(Driver)

add_executable(
        staticscript
        $<TARGET_OBJECTS:parser>
        $<TARGET_OBJECTS:entity>
        $<TARGET_OBJECTS:ast>
        $<TARGET_OBJECTS:sema>
        $<TARGET_OBJECTS:codegen>
        $<TARGET_OBJECTS:driver>
)

add_dependencies(staticscript antlr4_static)

target_link_libraries(staticscript PRIVATE antlr4_static ${llvm_libs})