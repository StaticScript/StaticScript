#include "PrimitiveType.h"

PrimitiveType::PrimitiveType(std::string name) : name(std::move(name)) {}

const std::string &PrimitiveType::getName() {
    return name;
}

const std::string &PrimitiveType::toString() {
    return name;
}

Scope *PrimitiveType::getEnclosingScope() {
    return nullptr;
}

bool PrimitiveType::isType(Type *type) {
    return this == type;
}

PrimitiveType *PrimitiveType::Boolean = new PrimitiveType("Boolean");
PrimitiveType *PrimitiveType::Number = new PrimitiveType("Number");
PrimitiveType *PrimitiveType::String = new PrimitiveType("String");