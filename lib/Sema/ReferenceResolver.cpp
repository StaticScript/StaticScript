#include "Sema/ReferenceResolver.h"

void ReferenceResolver::visit(const SharedPtr<ModuleNode> &module) {
    pushScope(module->internalScope);
    ASTVisitor::visit(module);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<VarDeclNode> &varDecl) {
    if (getCurrentScope()->hasVariable(varDecl->name)) {
        throw SemanticException("变量重复定义: " + varDecl->name);
    } else {
        getCurrentScope()->addVariable(varDecl->name, varDecl);
    }
    ASTVisitor::visit(varDecl);
}

void ReferenceResolver::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    getCurrentScope()->addVariable(paramVarDecl->name, paramVarDecl);
}

void ReferenceResolver::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    const SharedPtr<TopLevelScope> &topLevelScope = dynPtrCast<TopLevelScope>(getCurrentScope());
    if (topLevelScope->hasFunction(funcDecl->name)) {
        throw SemanticException("函数重复定义: " + funcDecl->name);
    } else {
        topLevelScope->addFunction(funcDecl->name, funcDecl);
    }
    pushScope(funcDecl->internalScope);
    ASTVisitor::visit(funcDecl);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    const SharedPtr<VarDeclNode> &varDecl = resolveVariable(varExpr->name);
    if (varDecl) {
        varExpr->refVarDecl = varDecl;
    } else {
        throw SemanticException("变量未定义: " + varExpr->name);
    }
}

void ReferenceResolver::visit(const SharedPtr<CallExprNode> &callExpr) {
    SharedPtr<FunctionDeclNode> funcDecl = resolveFunction(callExpr->calleeName);
    if (funcDecl) {
        callExpr->refFuncDecl = funcDecl;
    } else {
        throw SemanticException("函数未定义: " + callExpr->calleeName);
    }
    ASTVisitor::visit(callExpr);
}

void ReferenceResolver::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    pushScope(compStmt->internalScope);
    ASTVisitor::visit(compStmt);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<ForStmtNode> &forStmt) {
    pushScope(forStmt->internalScope);
    ASTVisitor::visit(forStmt);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    SharedPtr<Scope> scope = returnStmt->scope->getParent();
    SharedPtr<FunctionDeclNode> funcDecl;
    while (scope) {
        funcDecl = dynPtrCast<FunctionDeclNode>(scope->host);
        if (funcDecl) {
            break;
        }
        scope = scope->getParent();
    }
    if (funcDecl) {
        returnStmt->refFuncDecl = funcDecl;
        funcDecl->refReturnStmt = returnStmt;
    } else {
        // 这里本应该是语义合法性检查器的责任, 但是类型检查器要使用到refFuncDecl信息,所以提前到这里做
        throw SemanticException("return语句不在函数内");
    }
    ASTVisitor::visit(returnStmt);
}
