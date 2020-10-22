set(SRC IRGenerator.cpp)

add_library(codegen OBJECT ${SRC})

add_dependencies(codegen ast sema)
