#ifndef STATICSCRIPT_AST_ASTVISITOR_H
#define STATICSCRIPT_AST_ASTVISITOR_H

#include "AST/AST.h"

template<typename S, typename E>
class ASTVisitor {
public:
    virtual SharedPtr<S> visit(SharedPtr<CompoundStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ExprStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<VarDeclStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<FunctionDeclStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<IfStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<WhileStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ForStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ContinueStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<BreakStmt> stmt) = 0;
    virtual SharedPtr<S> visit(SharedPtr<ReturnStmt> stmt) = 0;

    virtual SharedPtr<E> visit(SharedPtr<BooleanLiteralExpr> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<IntegerLiteralExpr> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<StringLiteralExpr> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<IdentifierExpr> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<CallExpr> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<UnaryOperatorExpr> expr) = 0;
    virtual SharedPtr<E> visit(SharedPtr<BinaryOperatorExpr> expr) = 0;
};

#endif // STATICSCRIPT_AST_ASTVISITOR_H
