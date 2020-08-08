#include "AST/Type.h"

Type::Type(TypeKind kind) : kind(kind) {}

BuiltinType::BuiltinType(TypeKind kind) : Type(kind) {}