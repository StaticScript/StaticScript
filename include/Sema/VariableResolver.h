#pragma once

#include <stack>
#include "Entity/Scope.h"
#include "Sema/ASTVisitor.h"
#include "Exception/SemanticException.h"

class VariableResolver final : public ASTVisitor {
public:
    inline void resolve(const SharedPtr<ModuleNode> &module) {
        module->accept(getThisSharedPtr());
    }

private:
    void visit(const SharedPtr<ModuleNode> &module) override;

    void visit(const SharedPtr<VarDeclNode> &varDecl) override;

    void visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) override;

    void visit(const SharedPtr<FunctionDeclNode> &funcDecl) override;

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

    void visit(const SharedPtr<ReturnStmtNode> &returnStmt) override;

    inline SharedPtr<VariableResolver> getThisSharedPtr() {
        return staticPtrCast<VariableResolver>(shared_from_this());
    }

    inline void pushScope(const SharedPtr<Scope> &scope) {
        scopeStack.push(scope);
    }

    inline void popScope() {
        scopeStack.pop();
    }

    inline SharedPtr<Scope> getCurrentScope() {
        return scopeStack.top();
    }

    /// 查找变量, 沿着当前作用域递归向上查找
    inline SharedPtr<VarDeclNode> resolveVariable(const String &name) {
        SharedPtr<Scope> scope = getCurrentScope();
        return scope->resolveVariable(name);
    }

    inline SharedPtr<FunctionDeclNode> resolveFunction(const String &name) {
        SharedPtr<TopLevelScope> topLevelScope = getCurrentScope()->getTopLevel();
        return topLevelScope->resolveFunction(name);
    }

    std::stack<SharedPtr<Scope>> scopeStack;
};