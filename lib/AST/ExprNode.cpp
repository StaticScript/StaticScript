#include <utility>
#include "AST/ExprNode.h"
#include "AST/DeclNode.h"
#include "Sema/ASTVisitor.h"

LiteralExprNode::LiteralExprNode(TypeKind type) : type(type) {}

IntegerLiteralExprNode::IntegerLiteralExprNode(TypeKind type, int literal) : LiteralExprNode(type), literal(literal) {}

void IntegerLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IntegerLiteralExprNode>(shared_from_this()));
}

BooleanLiteralExprNode::BooleanLiteralExprNode(TypeKind type, bool literal) : LiteralExprNode(type), literal(literal) {}

void BooleanLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<BooleanLiteralExprNode>(shared_from_this()));
}

StringLiteralExprNode::StringLiteralExprNode(TypeKind type, String literal) : LiteralExprNode(type), literal(std::move(literal)) {}

void StringLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<StringLiteralExprNode>(shared_from_this()));
}

IdentifierExprNode::IdentifierExprNode(String name) : name(std::move(name)) {}

void IdentifierExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IdentifierExprNode>(shared_from_this()));
}

CallExprNode::CallExprNode(String calleeName, const SharedPtrVector<ExprNode> &args) : calleeName(std::move(calleeName)), args(args) {}

void CallExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<CallExprNode>(shared_from_this()));
}

UnaryOperatorExprNode::UnaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &subExpr) : opCode(opCode), subExpr(subExpr) {}

void UnaryOperatorExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<UnaryOperatorExprNode>(shared_from_this()));
}

BinaryOperatorExprNode::BinaryOperatorExprNode(
        unsigned int opCode,
        const SharedPtr<ExprNode> &lhs,
        const SharedPtr<ExprNode> &rhs
) : opCode(opCode),
    lhs(lhs),
    rhs(rhs) {}

void BinaryOperatorExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<BinaryOperatorExprNode>(shared_from_this()));
}




