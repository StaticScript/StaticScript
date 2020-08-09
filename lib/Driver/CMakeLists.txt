set(SRC Driver.cpp)

add_library(driver OBJECT ${SRC})

add_dependencies(driver parser ast)