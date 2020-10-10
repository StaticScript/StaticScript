#pragma once

#include "AST/AST.h"

template<typename S, typename E>
class ASTVisitor {
public:
    virtual SharedPtr<S> visit(SharedPtr<CompoundStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ExprStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<VarDeclStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<FunctionDeclStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<IfStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<WhileStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ForStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ContinueStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<BreakStmtNode> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ReturnStmtNode> stmt) = 0;

    virtual SharedPtr<E> visit(SharedPtr<BooleanLiteralExprNode> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<IntegerLiteralExprNode> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<StringLiteralExprNode> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<IdentifierExprNode> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<CallExprNode> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<UnaryOperatorExprNode> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<BinaryOperatorExprNode> expr) = 0;
};
