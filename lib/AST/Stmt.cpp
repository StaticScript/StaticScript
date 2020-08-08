#include "AST/Stmt.h"
#include "AST/Decl.h"

void VarDeclStmt::pushVarDecl(const SharedPtr<VarDecl> &varDecl) {
    childVarDecls.push_back(varDecl);
}

CompoundStmt::CompoundStmt(const SharedPtrVector<Stmt> &childStmts) : childStmts(childStmts) {}

FunctionDeclStmt::FunctionDeclStmt(const SharedPtr<FunctionDecl> &childFunctionDecl) : childFunctionDecl(childFunctionDecl) {}

IfStmt::IfStmt(const SharedPtr<Expr> &condition, const SharedPtr<Stmt> &thenStmt, const SharedPtr<Stmt> &elseStmt) : condition(condition),
                                                                                                                     thenBody(thenStmt),
                                                                                                                     elseBody(elseStmt) {}

WhileStmt::WhileStmt(const SharedPtr<Expr> &condition, const SharedPtr<Stmt> &body) : condition(condition), body(body) {}

ForStmt::ForStmt(const SharedPtr<VarDeclStmt> &forInitVarDecls, const SharedPtrVector<Expr> &forInitExprList, const SharedPtr<Expr> &forCondition,
                 const SharedPtrVector<Expr> &forUpdate, const SharedPtr<Stmt> &body) : forInitVarDecls(forInitVarDecls),
                                                                                        forInitExprList(forInitExprList), forCondition(forCondition),
                                                                                        forUpdate(forUpdate), body(body) {}

ReturnStmt::ReturnStmt(const SharedPtr<Expr> &argument) : argument(argument) {}
