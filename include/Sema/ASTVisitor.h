#pragma once

#include <memory>
#include "AST/AST.h"

/// AST遍历器
class ASTVisitor : public std::enable_shared_from_this<ASTVisitor> {
public:
    virtual void resolve(const SharedPtr<ModuleNode> &module) {
        module->accept(shared_from_this());
    }

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

protected:
    /**
     * @brief 将作用域压栈
     *
     * @param scope 作用域
     */
    inline void pushScope(const SharedPtr<Scope> &scope) {
        scopeStack.push(scope);
    }

    /**
     * @brief 作用域出栈
     */
    inline void popScope() {
        scopeStack.pop();
    }

    /**
     * @brief 获取当前作用域
     * @return 当前作用域
     */
    inline const SharedPtr<Scope> &getCurrentScope() {
        return scopeStack.top();
    }

    /// 作用域栈
    std::stack<SharedPtr<Scope>> scopeStack;
};