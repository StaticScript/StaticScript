set(CODEGEN_SRC IRGenerator.cpp DeclCodeGen.cpp ExprCodeGen.cpp StmtCodeGen.cpp)

set(SRC Builtin.cpp ${CODEGEN_SRC} Pass.cpp)

add_library(codegen OBJECT ${SRC})

add_dependencies(codegen sema lib)