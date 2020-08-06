set(SRC Base.cpp Decl.cpp Type.cpp Stmt.cpp Expr.cpp Module.cpp)

add_library(ast OBJECT ${SRC})

add_dependencies(ast parser)