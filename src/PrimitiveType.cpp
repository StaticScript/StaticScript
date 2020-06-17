#include "PrimitiveType.h"

PrimitiveType::PrimitiveType(std::string name) : name(std::move(name)) {}

const std::string &PrimitiveType::getName() const {
    return name;
}

const std::string &PrimitiveType::toString() const {
    return name;
}

Scope *PrimitiveType::getEnclosingScope() const {
    return nullptr;
}

bool PrimitiveType::isType(const Type *type) const {
    return this == type;
}

PrimitiveType *PrimitiveType::Boolean = new PrimitiveType("Boolean");
PrimitiveType *PrimitiveType::Number = new PrimitiveType("Number");
PrimitiveType *PrimitiveType::String = new PrimitiveType("String");