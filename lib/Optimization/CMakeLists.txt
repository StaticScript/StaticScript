set(SRC Optimizer.cpp)

add_library(optimization OBJECT ${SRC})

add_dependencies(optimization codegen)