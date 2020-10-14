#include "Sema/VariableResolver.h"

void VariableResolver::visit(const SharedPtr<ModuleNode> &module) {
    SharedPtr<TopLevelScope> topLevelScope = TopLevelScope::create();
    pushScope(topLevelScope);
    for (const SharedPtr<StmtNode> &stmt: module->childStmts) {
        stmt->accept(getThisSharedPtr());
    }
    popScope();
}

void VariableResolver::visit(const SharedPtr<VarDeclNode> &varDecl) {
    getCurrentScope()->addVariable(varDecl->name, varDecl);
}

void VariableResolver::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    getCurrentScope()->addVariable(paramVarDecl->name, paramVarDecl);
}

void VariableResolver::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    SharedPtr<TopLevelScope> scope = dynPtrCast<TopLevelScope>(getCurrentScope());
    if (scope) {
        scope->addFunction(funcDecl->name, funcDecl);
    } else {
        throw SemanticException("不允许函数嵌套");
    }
    /// 参数作用域: 在外层作作用域与内层作用域之间的辅助作用域
    SharedPtr<LocalScope> paramScope = LocalScope::create(scope);
    pushScope(paramScope);
    for (const SharedPtr<ParmVarDeclNode> &paramVarDecl : funcDecl->params) {
        paramVarDecl->accept(getThisSharedPtr());
    }
    funcDecl->body->accept(getThisSharedPtr());
    popScope();
}

void VariableResolver::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    SharedPtr<VarDeclNode> varDecl = resolveVariable(varExpr->name);
    if (varDecl) {
        varExpr->assocVarDecl = varDecl;
    } else {
        throw SemanticException("变量未定义: " + varExpr->name);
    }
}

void VariableResolver::visit(const SharedPtr<CallExprNode> &callExpr) {
    SharedPtr<FunctionDeclNode> funcDecl = resolveFunction(callExpr->calleeName);
    if (funcDecl) {
        callExpr->assocFuncDecl = funcDecl;
    } else {
        throw SemanticException("函数未定义: " + callExpr->calleeName);
    }
    for (const SharedPtr<ExprNode> &argExpr: callExpr->args) {
        argExpr->accept(getThisSharedPtr());
    }
}

void VariableResolver::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    uopExpr->subExpr->accept(getThisSharedPtr());
}

void VariableResolver::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    bopExpr->lhs->accept(getThisSharedPtr());
    bopExpr->rhs->accept(getThisSharedPtr());
}

void VariableResolver::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    exprStmt->expr->accept(getThisSharedPtr());
}

void VariableResolver::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    SharedPtr<LocalScope> localScope = LocalScope::create(getCurrentScope());
    pushScope(localScope);
    for (const SharedPtr<StmtNode> &stmt: compStmt->childStmts) {
        stmt->accept(getThisSharedPtr());
    }
    popScope();
}

void VariableResolver::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    for (const SharedPtr<VarDeclNode> &varDecl : varDeclStmt->childVarDecls) {
        varDecl->accept(getThisSharedPtr());
    }
}

void VariableResolver::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    funcDeclStmt->childFunctionDecl->accept(getThisSharedPtr());
}

void VariableResolver::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ifStmt->condition->accept(getThisSharedPtr());
    ifStmt->thenBody->accept(getThisSharedPtr());
    if (ifStmt->elseBody) {
        ifStmt->elseBody->accept(getThisSharedPtr());
    }
}

void VariableResolver::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    whileStmt->condition->accept(getThisSharedPtr());
    whileStmt->body->accept(getThisSharedPtr());
}

void VariableResolver::visit(const SharedPtr<ForStmtNode> &forStmt) {
    SharedPtr<LocalScope> forScope = LocalScope::create(getCurrentScope());
    pushScope(forScope);
    if (forStmt->forInitVarDecls) {
        forStmt->forInitVarDecls->accept(getThisSharedPtr());
    } else {
        for (const SharedPtr<ExprNode> &initExpr: forStmt->forInitExprList) {
            initExpr->accept(getThisSharedPtr());
        }
    }
    if (forStmt->forCondition) {
        forStmt->forCondition->accept(getThisSharedPtr());
    }
    for (const SharedPtr<ExprNode> &updateExpr: forStmt->forUpdate) {
        updateExpr->accept(getThisSharedPtr());
    }
    forStmt->body->accept(getThisSharedPtr());
    popScope();
}

void VariableResolver::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    if (returnStmt->returnExpr) {
        returnStmt->returnExpr->accept(getThisSharedPtr());
    }
}
