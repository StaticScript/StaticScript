#include <utility>
#include "AST/ExprNode.h"
#include "AST/DeclNode.h"
#include "Sema/ASTVisitor.h"

ExprNode::ExprNode(const SharedPtr<BuiltinTypeNode> &type) : type(type) {}

LiteralExprNode::LiteralExprNode(const SharedPtr<BuiltinTypeNode> &type) : ExprNode(type) {}

IntegerLiteralExprNode::IntegerLiteralExprNode(int literal) : literal(literal), LiteralExprNode(BuiltinTypeNode::INTEGER_TYPE) {}

void IntegerLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IntegerLiteralExprNode>(shared_from_this()));
}

BooleanLiteralExprNode::BooleanLiteralExprNode(bool literal) : literal(literal), LiteralExprNode(BuiltinTypeNode::BOOLEAN_TYPE) {}

void BooleanLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<BooleanLiteralExprNode>(shared_from_this()));
}

StringLiteralExprNode::StringLiteralExprNode(String literal) : literal(std::move(literal)), LiteralExprNode(BuiltinTypeNode::STRING_TYPE) {}

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




