#pragma once

#include <stack>
#include "Entity/Scope.h"
#include "Sema/ASTVisitor.h"
#include "Exception/SemanticException.h"

/// 引用消解器
class ReferenceResolver final : public ASTVisitor {
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

    inline SharedPtr<ReferenceResolver> getThisSharedPtr() {
        return staticPtrCast<ReferenceResolver>(shared_from_this());
    }

    inline void pushScope(const SharedPtr<Scope> &scope) {
        scopeStack.push(scope);
    }

    inline void popScope() {
        scopeStack.pop();
    }

    inline const SharedPtr<Scope> &getCurrentScope() {
        return scopeStack.top();
    }

    /**
     * @brief 查找变量
     * @details 沿着当前作用域递归向上查找变量
     *
     * @param name 变量名
     * @return 变量定义节点
     */
    inline SharedPtr<VarDeclNode> resolveVariable(const String &name) {
        return getCurrentScope()->resolveVariable(name);
    }

    /**
     * @brief 查找函数
     * @details 在顶级作用域中查找函数
     *
     * @param name 函数名
     * @return 函数定义节点
     */
    inline SharedPtr<FunctionDeclNode> resolveFunction(const String &name) {
        return getCurrentScope()->getTopLevel()->resolveFunction(name);
    }

    /// 作用域栈
    std::stack<SharedPtr<Scope>> scopeStack;
};