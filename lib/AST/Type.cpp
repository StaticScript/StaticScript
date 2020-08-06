#include "AST/Type.h"

Type::Type(TypeKind kind) : kind(kind) {}

BuiltinType::BuiltinType(TypeKind kind) : Type(kind) {}

void BuiltinType::codegen() {}

ReferenceType::ReferenceType() : Type(TypeKind::Reference) {}

void ReferenceType::codegen() {}
