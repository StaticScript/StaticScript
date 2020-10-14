#include "Sema/ReferenceResolver.h"

void ReferenceResolver::visit(const SharedPtr<ModuleNode> &module) {
    pushScope(module->internalScope);
    for (const SharedPtr<StmtNode> &stmt: module->childStmts) {
        stmt->accept(getThisSharedPtr());
    }
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
    for (const SharedPtr<ParmVarDeclNode> &paramVarDecl : funcDecl->params) {
        paramVarDecl->accept(getThisSharedPtr());
    }
    funcDecl->body->accept(getThisSharedPtr());
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
    for (const SharedPtr<ExprNode> &argExpr: callExpr->args) {
        argExpr->accept(getThisSharedPtr());
    }
}

void ReferenceResolver::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    uopExpr->subExpr->accept(getThisSharedPtr());
}

void ReferenceResolver::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    bopExpr->lhs->accept(getThisSharedPtr());
    bopExpr->rhs->accept(getThisSharedPtr());
}

void ReferenceResolver::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    exprStmt->expr->accept(getThisSharedPtr());
}

void ReferenceResolver::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    pushScope(compStmt->internalScope);
    for (const SharedPtr<StmtNode> &stmt: compStmt->childStmts) {
        stmt->accept(getThisSharedPtr());
    }
    popScope();
}

void ReferenceResolver::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    for (const SharedPtr<VarDeclNode> &varDecl : varDeclStmt->childVarDecls) {
        varDecl->accept(getThisSharedPtr());
    }
}

void ReferenceResolver::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    funcDeclStmt->childFunctionDecl->accept(getThisSharedPtr());
}

void ReferenceResolver::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ifStmt->condition->accept(getThisSharedPtr());
    ifStmt->thenBody->accept(getThisSharedPtr());
    if (ifStmt->elseBody) {
        ifStmt->elseBody->accept(getThisSharedPtr());
    }
}

void ReferenceResolver::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    whileStmt->condition->accept(getThisSharedPtr());
    whileStmt->body->accept(getThisSharedPtr());
}

void ReferenceResolver::visit(const SharedPtr<ForStmtNode> &forStmt) {
    pushScope(forStmt->internalScope);
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

void ReferenceResolver::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    if (returnStmt->returnExpr) {
        returnStmt->returnExpr->accept(getThisSharedPtr());
    }
}
