#pragma once

#include "Entity/Scope.h"
#include "Sema/ASTVisitor.h"
#include "Exception/SemanticException.h"

/// 作用域扫描器
class ScopeScanner final : public ASTVisitor {
public:
    inline void resolve(const SharedPtr<ModuleNode> &module) {
        module->accept(getThisSharedPtr());
    }

private:
    void visit(const SharedPtr<ModuleNode> &module) override;

    void visit(const SharedPtr<VarDeclNode> &varDecl) override;

    void visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) override;

    void visit(const SharedPtr<FunctionDeclNode> &funcDecl) override;

    void visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr) override;

    void visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr) override;

    void visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) override;

    void visit(const SharedPtr<IdentifierExprNode> &varExpr) override;

    void visit(const SharedPtr<CallExprNode> &callExpr) override;

    void visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) override;

    void visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) override;

    void visit(const SharedPtr<ExprStmtNode> &exprStmt) override;

    void visit(const SharedPtr<CompoundStmtNode> &compStmt) override;

    void visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) override;

    void visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) override;

    void visit(const SharedPtr<IfStmtNode> &ifStmt) override;

    void visit(const SharedPtr<WhileStmtNode> &whileStmt) override;

    void visit(const SharedPtr<ForStmtNode> &forStmt) override;

    void visit(const SharedPtr<ContinueStmtNode> &continueStmt) override;

    void visit(const SharedPtr<BreakStmtNode> &breakStmt) override;

    void visit(const SharedPtr<ReturnStmtNode> &returnStmt) override;

    /**
     * @brief 获取当前扫描器实例的智能指针
     * @return 当前扫描器实例的智能指针
     */
    inline SharedPtr<ScopeScanner> getThisSharedPtr() {
        return staticPtrCast<ScopeScanner>(shared_from_this());
    }

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