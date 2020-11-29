set(SRC DeclNode.cpp StmtNode.cpp ExprNode.cpp ModuleNode.cpp ASTBuilder.cpp)

add_library(ast OBJECT ${SRC})

add_dependencies(ast parser entity)