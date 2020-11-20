#pragma once

#include "StaticScriptLexer.h"
#include "Sema/ASTVisitor.h"
#include "Support/Exception.h"

/// 类型检查器
class TypeChecker : public ASTVisitor {
public:
    void visit(const SharedPtr<VarDeclNode> &varDecl) override;

    void visit(const SharedPtr<IdentifierExprNode> &varExpr) override;

    void visit(const SharedPtr<CallExprNode> &callExpr) override;

    void visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) override;

    void visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) override;

    void visit(const SharedPtr<IfStmtNode> &ifStmt) override;

    void visit(const SharedPtr<WhileStmtNode> &whileStmt) override;

    void visit(const SharedPtr<ForStmtNode> &forStmt) override;

    void visit(const SharedPtr<ReturnStmtNode> &returnStmt) override;
};