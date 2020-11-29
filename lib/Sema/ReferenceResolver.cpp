#include "Sema/ReferenceResolver.h"

void ReferenceResolver::visit(const SharedPtr<ModuleNode> &module) {
    pushScope(module->internalScope);
    ASTVisitor::visit(module);
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<VarDeclNode> &varDecl) {
    if (getCurrentScope()->hasVariable(varDecl->name)) {
        reportSemanticError("Variable repeated definition: " + varDecl->name);
    } else {
        getCurrentScope()->addVariable(varDecl->name, varDecl);
    }
    ASTVisitor::visit(varDecl);
}

void ReferenceResolver::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    if (getCurrentScope()->hasVariable(paramVarDecl->name)) {
        reportSemanticError("Repeated definition of function parameters: " + paramVarDecl->name);
    } else {
        getCurrentScope()->addVariable(paramVarDecl->name, paramVarDecl);
    }
}

void ReferenceResolver::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    const SharedPtr<TopLevelScope> &topLevelScope = dynPtrCast<TopLevelScope>(getCurrentScope());
    if (topLevelScope->hasFunction(funcDecl->name)) {
        reportSemanticError("Function repeated definition: " + funcDecl->name);
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
        reportSemanticError("Variable undefined: " + varExpr->name);
    }
}

void ReferenceResolver::visit(const SharedPtr<CallExprNode> &callExpr) {
    SharedPtr<FunctionDeclNode> funcDecl = resolveFunction(callExpr->calleeName);
    if (funcDecl) {
        callExpr->refFuncDecl = funcDecl;
    } else {
        reportSemanticError("Function undefined: " + callExpr->calleeName);
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

void ReferenceResolver::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {
    continueStmt->refIterationStmt = resolveRefIterationStmt(continueStmt);
}

void ReferenceResolver::visit(const SharedPtr<BreakStmtNode> &breakStmt) {
    breakStmt->refIterationStmt = resolveRefIterationStmt(breakStmt);
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
        reportSemanticError("The return statement muse be in a function");
    }
    ASTVisitor::visit(returnStmt);
}

SharedPtr<StmtNode> ReferenceResolver::resolveRefIterationStmt(const SharedPtr<Node> &node) {
    SharedPtr<Node> iterNode = node->parent;
    SharedPtr<WhileStmtNode> refWhileStmt = nullptr;
    SharedPtr<ForStmtNode> refForStmt = nullptr;
    while (iterNode) {
        SharedPtr<WhileStmtNode> whileStmt = dynPtrCast<WhileStmtNode>(iterNode);
        if (whileStmt) {
            refWhileStmt = whileStmt;
            break;
        }
        SharedPtr<ForStmtNode> forStmt = dynPtrCast<ForStmtNode>(iterNode);
        if (forStmt) {
            refForStmt = forStmt;
            break;
        }
        iterNode = iterNode->parent;
    }
    if (refWhileStmt) {
        return refWhileStmt;
    }
    return refForStmt;
}
