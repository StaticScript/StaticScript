set(SRC Builtin.cpp IRGenerator.cpp Pass.cpp)

add_library(codegen OBJECT ${SRC})

add_dependencies(codegen ast entity sema external_bitcode)