#include "AST/StmtNode.h"
#include "AST/DeclNode.h"

ExprStmtNode::ExprStmtNode(const SharedPtr<ExprNode> &expr) : expr(expr) {}

CompoundStmtNode::CompoundStmtNode(const SharedPtrVector<StmtNode> &childStmts) : childStmts(childStmts) {}

bool CompoundStmtNode::isEmpty() const {
    return childStmts.empty();
}

void VarDeclStmtNode::pushVarDecl(const SharedPtr<VarDeclNode> &varDecl) {
    childVarDecls.push_back(varDecl);
}

FunctionDeclStmtNode::FunctionDeclStmtNode(const SharedPtr<FunctionDeclNode> &childFunctionDecl) : childFunctionDecl(childFunctionDecl) {}

IfStmtNode::IfStmtNode(const SharedPtr<ExprNode> &condition, const SharedPtr<StmtNode> &thenStmt, const SharedPtr<StmtNode> &elseStmt) : condition(condition),
                                                                                                                                         thenBody(thenStmt),
                                                                                                                                         elseBody(elseStmt) {}

WhileStmtNode::WhileStmtNode(const SharedPtr<ExprNode> &condition, const SharedPtr<StmtNode> &body) : condition(condition), body(body) {}

ForStmtNode::ForStmtNode(const SharedPtr<VarDeclStmtNode> &forInitVarDecls, const SharedPtrVector<ExprNode> &forInitExprList, const SharedPtr<ExprNode> &forCondition,
                         const SharedPtrVector<ExprNode> &forUpdate, const SharedPtr<StmtNode> &body) : forInitVarDecls(forInitVarDecls),
                                                                                                forInitExprList(forInitExprList), forCondition(forCondition),
                                                                                                forUpdate(forUpdate), body(body) {}

ReturnStmtNode::ReturnStmtNode(const SharedPtr<ExprNode> &argument) : argument(argument) {}
