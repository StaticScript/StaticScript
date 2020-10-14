#include "AST/StmtNode.h"
#include "AST/DeclNode.h"
#include "Sema/ASTVisitor.h"

ExprStmtNode::ExprStmtNode(const SharedPtr<ExprNode> &expr) : expr(expr) {}

void ExprStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ExprStmtNode>(shared_from_this()));
}

CompoundStmtNode::CompoundStmtNode(const SharedPtrVector<StmtNode> &childStmts) : childStmts(childStmts) {}

bool CompoundStmtNode::isEmpty() const {
    return childStmts.empty();
}

void CompoundStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<CompoundStmtNode>(shared_from_this()));
}


void VarDeclStmtNode::pushVarDecl(const SharedPtr<VarDeclNode> &varDecl) {
    childVarDecls.push_back(varDecl);
}

void VarDeclStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<VarDeclStmtNode>(shared_from_this()));
}

FunctionDeclStmtNode::FunctionDeclStmtNode(
        const SharedPtr<FunctionDeclNode> &childFunctionDecl
) : childFunctionDecl(childFunctionDecl) {}

void FunctionDeclStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<FunctionDeclStmtNode>(shared_from_this()));
}

IfStmtNode::IfStmtNode(
        const SharedPtr<ExprNode> &condition,
        const SharedPtr<StmtNode> &thenStmt,
        const SharedPtr<StmtNode> &elseStmt
) : condition(condition), thenBody(thenStmt), elseBody(elseStmt) {}

void IfStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<IfStmtNode>(shared_from_this()));
}

WhileStmtNode::WhileStmtNode(
        const SharedPtr<ExprNode> &condition,
        const SharedPtr<StmtNode> &body
) : condition(condition), body(body) {}

void WhileStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<WhileStmtNode>(shared_from_this()));
}

ForStmtNode::ForStmtNode(
        const SharedPtr<VarDeclStmtNode> &forInitVarDecls,
        const SharedPtrVector<ExprNode> &forInitExprList,
        const SharedPtr<ExprNode> &forCondition,
        const SharedPtrVector<ExprNode> &forUpdate,
        const SharedPtr<StmtNode> &body
) : forInitVarDecls(forInitVarDecls),
    forInitExprList(forInitExprList),
    forCondition(forCondition),
    forUpdate(forUpdate), body(body) {}

void ForStmtNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ForStmtNode>(shared_from_this()));
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
