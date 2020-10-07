#include "AST/TypeNode.h"

TypeNode::TypeNode(TypeKind kind) : kind(kind) {}

BuiltinTypeNode::BuiltinTypeNode(TypeKind kind) : TypeNode(kind) {}