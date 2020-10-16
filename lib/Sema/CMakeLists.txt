set(SRC ASTVisitor.cpp ScopeScanner.cpp ReferenceResolver.cpp TypeChecker.cpp SemanticValidator.cpp)

add_library(sema OBJECT ${SRC})

add_dependencies(sema ast)