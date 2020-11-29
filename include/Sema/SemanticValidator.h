#pragma once

#include "StaticScriptLexer.h"
#include "Sema/ASTVisitor.h"

/// 语义合法性验证器
class SemanticValidator : public ASTVisitor {
public:
    void visit(const SharedPtr<VarDeclNode> &varDecl) override;

    void visit(const SharedPtr<FunctionDeclNode> &funcDecl) override;

    void visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) override;

    void visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) override;

    void visit(const SharedPtr<ContinueStmtNode> &continueStmt) override;

    void visit(const SharedPtr<BreakStmtNode> &breakStmt) override;
};
