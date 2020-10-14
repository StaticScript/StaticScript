#include "AST/TypeNode.h"
#include "Sema/ASTVisitor.h"

TypeNode::TypeNode(TypeKind kind) : kind(kind) {}

BuiltinTypeNode::BuiltinTypeNode(TypeKind kind) : TypeNode(kind) {}

bool BuiltinTypeNode::isBuiltin() const {
    return true;
}

void BuiltinTypeNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<BuiltinTypeNode>(shared_from_this()));
}
