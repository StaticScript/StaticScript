#include <utility>
#include "AST/ExprNode.h"
#include "Sema/ASTVisitor.h"

ExprNode::ExprNode(const SharedPtr<Type> &type) : type(type) {}

LiteralExprNode::LiteralExprNode(const SharedPtr<Type> &type) : ExprNode(type) {}

BooleanLiteralExprNode::BooleanLiteralExprNode(bool literal) : literal(literal), LiteralExprNode(BasicType::BOOLEAN_TYPE) {}

void BooleanLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<BooleanLiteralExprNode>(shared_from_this()));
}

IntegerLiteralExprNode::IntegerLiteralExprNode(long literal) : literal(literal), LiteralExprNode(BasicType::INTEGER_TYPE) {}

void IntegerLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IntegerLiteralExprNode>(shared_from_this()));
}

FloatLiteralExprNode::FloatLiteralExprNode(double literal) : literal(literal), LiteralExprNode(BasicType::FLOAT_TYPE) {}

void FloatLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<FloatLiteralExprNode>(shared_from_this()));
}

StringLiteralExprNode::StringLiteralExprNode() : LiteralExprNode(BasicType::STRING_TYPE) {}

StringLiteralExprNode::StringLiteralExprNode(String literal) : literal(std::move(literal)), LiteralExprNode(BasicType::STRING_TYPE) {}

void StringLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<StringLiteralExprNode>(shared_from_this()));
}

ArrayLiteralExprNode::ArrayLiteralExprNode() : LiteralExprNode(BasicType::UNKNOWN_TYPE) {}

ArrayLiteralExprNode::ArrayLiteralExprNode(const SharedPtr<Type> &type) : LiteralExprNode(type) {}

void ArrayLiteralExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ArrayLiteralExprNode>(shared_from_this()));
}

IdentifierExprNode::IdentifierExprNode(String name) : name(std::move(name)) {}

void IdentifierExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IdentifierExprNode>(shared_from_this()));
}

CallExprNode::CallExprNode(String calleeName, const SharedPtrVector<ExprNode> &args) : calleeName(std::move(calleeName)), args(args) {}

void CallExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<CallExprNode>(shared_from_this()));
}

void CallExprNode::bindChildrenInversely() {
    auto self = shared_from_this();
    for (const SharedPtr<ExprNode> &arg: args) {
        arg->parent = self;
    }
}

UnaryOperatorExprNode::UnaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &subExpr) : opCode(opCode), subExpr(subExpr) {}

void UnaryOperatorExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<UnaryOperatorExprNode>(shared_from_this()));
}

void UnaryOperatorExprNode::bindChildrenInversely() {
    subExpr->parent = shared_from_this();
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

void BinaryOperatorExprNode::bindChildrenInversely() {
    lhs->parent = rhs->parent = shared_from_this();
}

void ArraySubscriptExprNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ArraySubscriptExprNode>(shared_from_this()));
}

ArraySubscriptExprNode::ArraySubscriptExprNode(
        const SharedPtr<ExprNode> &baseExpr,
        const SharedPtrVector<ExprNode> &indexExprs
) : baseExpr(baseExpr), indexExprs(indexExprs) {}
