set(SRC ASTVisitor.cpp ReferenceResolver.cpp ScopeScanner.cpp)

add_library(sema OBJECT ${SRC})

add_dependencies(sema ast)