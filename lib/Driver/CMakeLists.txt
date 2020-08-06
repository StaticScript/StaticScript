set(SRC Driver.cpp Visitor.cpp)

add_library(driver OBJECT ${SRC})

add_dependencies(driver parser ast)