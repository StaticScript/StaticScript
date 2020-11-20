set(CMAKE_CXX_FLAGS "-fvisibility=default -fvisibility-inlines-hidden")

set(SRC Driver.cpp)

add_library(driver OBJECT ${SRC})

add_dependencies(driver optimization)