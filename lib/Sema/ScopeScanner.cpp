#include "Sema/ScopeScanner.h"

void ScopeScanner::visit(const SharedPtr<ModuleNode> &module) {
    SharedPtr<TopLevelScope> topLevelScope = TopLevelScope::create();
    module->internalScope = topLevelScope;
    pushScope(topLevelScope);
    for (const SharedPtr<StmtNode> &stmt: module->childStmts) {
        stmt->accept(getThisSharedPtr());
    }
    popScope();
}

void ScopeScanner::visit(const SharedPtr<VarDeclNode> &varDecl) {
    varDecl->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    paramVarDecl->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    SharedPtr<TopLevelScope> topLevelScope = dynPtrCast<TopLevelScope>(getCurrentScope());
    if (topLevelScope) {
        funcDecl->scope = topLevelScope;
    } else {
        throw SemanticException("不允许函数嵌套");
    }
    // 函数参数作用域: 在外层作作用域与内层作用域之间的辅助作用域
    SharedPtr<LocalScope> paramScope = LocalScope::create(topLevelScope);
    funcDecl->internalScope = paramScope;
    pushScope(paramScope);
    for (const SharedPtr<ParmVarDeclNode> &paramVarDecl : funcDecl->params) {
        paramVarDecl->accept(getThisSharedPtr());
    }
    funcDecl->body->accept(getThisSharedPtr());
    popScope();
}

void ScopeScanner::visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr) {
    boolLiteralExpr->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr) {
    intLiteralExpr->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) {
    strLiteralExpr->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    varExpr->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<CallExprNode> &callExpr) {
    callExpr->scope = getCurrentScope();
    for (const SharedPtr<ExprNode> &argExpr: callExpr->args) {
        argExpr->accept(getThisSharedPtr());
    }
}

void ScopeScanner::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    uopExpr->scope = getCurrentScope();
    uopExpr->subExpr->accept(getThisSharedPtr());
}

void ScopeScanner::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    bopExpr->scope = getCurrentScope();
    bopExpr->lhs->accept(getThisSharedPtr());
    bopExpr->rhs->accept(getThisSharedPtr());
}

void ScopeScanner::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    exprStmt->scope = getCurrentScope();
    exprStmt->expr->accept(getThisSharedPtr());
}

void ScopeScanner::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    compStmt->scope = getCurrentScope();
    SharedPtr<LocalScope> localScope = LocalScope::create(getCurrentScope());
    compStmt->internalScope = localScope;
    pushScope(localScope);
    for (const SharedPtr<StmtNode> &stmt: compStmt->childStmts) {
        stmt->accept(getThisSharedPtr());
    }
    popScope();
}

void ScopeScanner::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    varDeclStmt->scope = getCurrentScope();
    for (const SharedPtr<VarDeclNode> &varDecl : varDeclStmt->childVarDecls) {
        varDecl->accept(getThisSharedPtr());
    }
}

void ScopeScanner::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    funcDeclStmt->scope = getCurrentScope();
    funcDeclStmt->childFunctionDecl->accept(getThisSharedPtr());
}

void ScopeScanner::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ifStmt->scope = getCurrentScope();
    ifStmt->condition->accept(getThisSharedPtr());
    ifStmt->thenBody->accept(getThisSharedPtr());
    if (ifStmt->elseBody) {
        ifStmt->elseBody->accept(getThisSharedPtr());
    }
}

void ScopeScanner::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    whileStmt->scope = getCurrentScope();
    whileStmt->condition->accept(getThisSharedPtr());
    whileStmt->body->accept(getThisSharedPtr());
}

void ScopeScanner::visit(const SharedPtr<ForStmtNode> &forStmt) {
    forStmt->scope = getCurrentScope();
    SharedPtr<LocalScope> forScope = LocalScope::create(getCurrentScope());
    forStmt->internalScope = forScope;
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

void ScopeScanner::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {
    continueStmt->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<BreakStmtNode> &breakStmt) {
    breakStmt->scope = getCurrentScope();
}

void ScopeScanner::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    returnStmt->scope = getCurrentScope();
    if (returnStmt->returnExpr) {
        returnStmt->returnExpr->accept(getThisSharedPtr());
    }
}
