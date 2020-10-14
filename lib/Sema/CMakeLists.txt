set(SRC ASTVisitor.cpp VariableResolver.cpp)

add_library(sema OBJECT ${SRC})

add_dependencies(sema ast)