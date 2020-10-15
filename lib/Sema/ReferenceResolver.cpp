#include "Sema/ReferenceResolver.h"

void ReferenceResolver::visit(const SharedPtr<ModuleNode> &module) {
    pushScope(module->internalScope);
    ASTVisitor::visit(module);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<VarDeclNode> &varDecl) {
    getCurrentScope()->addVariable(varDecl->name, varDecl);
}

void ReferenceResolver::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    getCurrentScope()->addVariable(paramVarDecl->name, paramVarDecl);
}

void ReferenceResolver::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    const SharedPtr<TopLevelScope> &topLevelScope = dynPtrCast<TopLevelScope>(getCurrentScope());
    topLevelScope->addFunction(funcDecl->name, funcDecl);
    pushScope(funcDecl->internalScope);
    ASTVisitor::visit(funcDecl);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    const SharedPtr<VarDeclNode> &varDecl = resolveVariable(varExpr->name);
    if (varDecl) {
        varExpr->assocVarDecl = varDecl;
    } else {
        throw SemanticException("变量未定义: " + varExpr->name);
    }
}

void ReferenceResolver::visit(const SharedPtr<CallExprNode> &callExpr) {
    SharedPtr<FunctionDeclNode> funcDecl = resolveFunction(callExpr->calleeName);
    if (funcDecl) {
        callExpr->assocFuncDecl = funcDecl;
    } else {
        throw SemanticException("函数未定义: " + callExpr->calleeName);
    }
    ASTVisitor::visit(callExpr);
}

void ReferenceResolver::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);
}

void ReferenceResolver::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);
}

void ReferenceResolver::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    ASTVisitor::visit(exprStmt);
}

void ReferenceResolver::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    pushScope(compStmt->internalScope);
    ASTVisitor::visit(compStmt);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    ASTVisitor::visit(varDeclStmt);
}

void ReferenceResolver::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    ASTVisitor::visit(funcDeclStmt);
}

void ReferenceResolver::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ASTVisitor::visit(ifStmt);
}

void ReferenceResolver::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    ASTVisitor::visit(whileStmt);
}

void ReferenceResolver::visit(const SharedPtr<ForStmtNode> &forStmt) {
    pushScope(forStmt->internalScope);
    ASTVisitor::visit(forStmt);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
}
