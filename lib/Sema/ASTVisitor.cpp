#include "Sema/ASTVisitor.h"

void ASTVisitor::resolve(const SharedPtr<ModuleNode> &module)  {
    module->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<ModuleNode> &module) {
    for (const SharedPtr<StmtNode> &stmt: module->childStmts) {
        stmt->accept(shared_from_this());
    }
}

void ASTVisitor::visit(const SharedPtr<BuiltinTypeNode> &builtinType) {}

void ASTVisitor::visit(const SharedPtr<VarDeclNode> &varDecl) {
    if(varDecl->initVal) {
        varDecl->initVal->accept(shared_from_this());
    }
}

void ASTVisitor::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {}

void ASTVisitor::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    for (const SharedPtr<ParmVarDeclNode> &paramVarDecl : funcDecl->params) {
        paramVarDecl->accept(shared_from_this());
    }
    funcDecl->body->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr) {}

void ASTVisitor::visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr) {}

void ASTVisitor::visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) {}

void ASTVisitor::visit(const SharedPtr<IdentifierExprNode> &varExpr) {}

void ASTVisitor::visit(const SharedPtr<CallExprNode> &callExpr) {
    for (const SharedPtr<ExprNode> &argExpr: callExpr->args) {
        argExpr->accept(shared_from_this());
    }
}

void ASTVisitor::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    uopExpr->subExpr->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    bopExpr->lhs->accept(shared_from_this());
    bopExpr->rhs->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    exprStmt->expr->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    for (const SharedPtr<StmtNode> &stmt: compStmt->childStmts) {
        stmt->accept(shared_from_this());
    }
}

void ASTVisitor::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    for (const SharedPtr<VarDeclNode> &varDecl : varDeclStmt->childVarDecls) {
        varDecl->accept(shared_from_this());
    }
}

void ASTVisitor::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    funcDeclStmt->childFunctionDecl->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ifStmt->condition->accept(shared_from_this());
    ifStmt->thenBody->accept(shared_from_this());
    if (ifStmt->elseBody) {
        ifStmt->elseBody->accept(shared_from_this());
    }
}

void ASTVisitor::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    whileStmt->condition->accept(shared_from_this());
    whileStmt->body->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<ForStmtNode> &forStmt) {
    if (forStmt->forInitVarDecls) {
        forStmt->forInitVarDecls->accept(shared_from_this());
    } else {
        for (const SharedPtr<ExprNode> &initExpr: forStmt->forInitExprList) {
            initExpr->accept(shared_from_this());
        }
    }
    if (forStmt->forCondition) {
        forStmt->forCondition->accept(shared_from_this());
    }
    for (const SharedPtr<ExprNode> &updateExpr: forStmt->forUpdate) {
        updateExpr->accept(shared_from_this());
    }
    forStmt->body->accept(shared_from_this());
}

void ASTVisitor::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {}

void ASTVisitor::visit(const SharedPtr<BreakStmtNode> &breakStmt) {}

void ASTVisitor::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    if (returnStmt->returnExpr) {
        returnStmt->returnExpr->accept(shared_from_this());
    }
}
