#pragma once

#include <memory>
#include "AST/AST.h"

/// AST遍历器
class ASTVisitor : public std::enable_shared_from_this<ASTVisitor> {
public:
    virtual void visit(const SharedPtr<ModuleNode> &module);

    virtual void visit(const SharedPtr<BuiltinTypeNode> &builtinType);

    virtual void visit(const SharedPtr<VarDeclNode> &varDecl);

    virtual void visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl);

    virtual void visit(const SharedPtr<FunctionDeclNode> &funcDecl);

    virtual void visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr);

    virtual void visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr);

    virtual void visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr);

    virtual void visit(const SharedPtr<IdentifierExprNode> &varExpr);

    virtual void visit(const SharedPtr<CallExprNode> &callExpr);

    virtual void visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr);

    virtual void visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr);

    virtual void visit(const SharedPtr<ExprStmtNode> &exprStmt);

    virtual void visit(const SharedPtr<CompoundStmtNode> &compStmt);

    virtual void visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt);

    virtual void visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt);

    virtual void visit(const SharedPtr<IfStmtNode> &ifStmt);

    virtual void visit(const SharedPtr<WhileStmtNode> &whileStmt);

    virtual void visit(const SharedPtr<ForStmtNode> &forStmt);

    virtual void visit(const SharedPtr<ContinueStmtNode> &continueStmt);

    virtual void visit(const SharedPtr<BreakStmtNode> &breakStmt);

    virtual void visit(const SharedPtr<ReturnStmtNode> &returnStmt);
};