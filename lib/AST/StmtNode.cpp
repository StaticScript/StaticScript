#include "AST/StmtNode.h"
#include "AST/DeclNode.h"
#include "Sema/ASTVisitor.h"

ExprStmtNode::ExprStmtNode(const SharedPtr<ExprNode> &expr) : expr(expr) {}

void ExprStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ExprStmtNode>(shared_from_this()));
}

void ExprStmtNode::bindChildrenInversely() {
    expr->parent = shared_from_this();
}

CompoundStmtNode::CompoundStmtNode(const SharedPtrVector<StmtNode> &childStmts) : childStmts(childStmts) {}

bool CompoundStmtNode::isEmpty() const {
    return childStmts.empty();
}

void CompoundStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<CompoundStmtNode>(shared_from_this()));
}

void CompoundStmtNode::bindChildrenInversely() {
    auto self = shared_from_this();
    for (const SharedPtr<StmtNode> &stmt: childStmts) {
        stmt->parent = self;
    }
}


void VarDeclStmtNode::pushVarDecl(const SharedPtr<VarDeclNode> &varDecl) {
    childVarDecls.push_back(varDecl);
}

void VarDeclStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<VarDeclStmtNode>(shared_from_this()));
}

void VarDeclStmtNode::bindChildrenInversely() {
    auto self = shared_from_this();
    for (const SharedPtr<VarDeclNode> &varDecl: childVarDecls) {
        varDecl->parent = self;
    }
}

FunctionDeclStmtNode::FunctionDeclStmtNode(
        const SharedPtr<FunctionDeclNode> &childFunctionDecl
) : childFunctionDecl(childFunctionDecl) {}

void FunctionDeclStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<FunctionDeclStmtNode>(shared_from_this()));
}

void FunctionDeclStmtNode::bindChildrenInversely() {
    childFunctionDecl->parent = shared_from_this();
}

IfStmtNode::IfStmtNode(
        const SharedPtr<ExprNode> &condition,
        const SharedPtr<StmtNode> &thenStmt,
        const SharedPtr<StmtNode> &elseStmt
) : condition(condition), thenBody(thenStmt), elseBody(elseStmt) {}

void IfStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IfStmtNode>(shared_from_this()));
}

void IfStmtNode::bindChildrenInversely() {
    auto self = shared_from_this();
    condition->parent = thenBody->parent = self;
    if (elseBody) {
        elseBody->parent = self;
    }
}

WhileStmtNode::WhileStmtNode(
        const SharedPtr<ExprNode> &condition,
        const SharedPtr<StmtNode> &body
) : condition(condition), body(body) {}

void WhileStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<WhileStmtNode>(shared_from_this()));
}

void WhileStmtNode::bindChildrenInversely() {
    condition->parent = body->parent = shared_from_this();
}

ForStmtNode::ForStmtNode(
        const SharedPtr<VarDeclStmtNode> &forInitVarDecls,
        const SharedPtrVector<ExprNode> &forInitExprList,
        const SharedPtr<ExprNode> &forCondition,
        const SharedPtrVector<ExprNode> &forUpdate,
        const SharedPtr<StmtNode> &body
) : initVarStmt(forInitVarDecls),
    initExprs(forInitExprList),
    condition(forCondition),
    updates(forUpdate), body(body) {}

void ForStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ForStmtNode>(shared_from_this()));
}

void ForStmtNode::bindChildrenInversely() {
    auto self = shared_from_this();
    if (initVarStmt) {
        initVarStmt->parent = self;
    }
    for (const SharedPtr<ExprNode> &forInitExpr: initExprs) {
        forInitExpr->parent = self;
    }
    if (condition) {
        condition->parent = self;
    }
    for (const SharedPtr<ExprNode> &forUpdateItem: updates) {
        forUpdateItem->parent = self;
    }
    body->parent = self;
}

void ContinueStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ContinueStmtNode>(shared_from_this()));
}

void BreakStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<BreakStmtNode>(shared_from_this()));
}

ReturnStmtNode::ReturnStmtNode(const SharedPtr<ExprNode> &argument) : returnExpr(argument) {}

void ReturnStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ReturnStmtNode>(shared_from_this()));
}

void ReturnStmtNode::bindChildrenInversely() {
    if (returnExpr) {
        returnExpr->parent = shared_from_this();
    }
}
