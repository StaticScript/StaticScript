#include "Sema/ASTVisitor.h"

void ASTVisitor::visit(const SharedPtr<ModuleNode> &module) {}

void ASTVisitor::visit(const SharedPtr<BuiltinTypeNode> &type) {}

void ASTVisitor::visit(const SharedPtr<VarDeclNode> &decl) {}

void ASTVisitor::visit(const SharedPtr<ParmVarDeclNode> &decl) {}

void ASTVisitor::visit(const SharedPtr<FunctionDeclNode> &decl) {}

void ASTVisitor::visit(const SharedPtr<BooleanLiteralExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<IntegerLiteralExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<StringLiteralExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<IdentifierExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<CallExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<UnaryOperatorExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<BinaryOperatorExprNode> &expr) {}

void ASTVisitor::visit(const SharedPtr<ExprStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<CompoundStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<VarDeclStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<FunctionDeclStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<IfStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<WhileStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<ForStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<ContinueStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<BreakStmtNode> &stmt) {}

void ASTVisitor::visit(const SharedPtr<ReturnStmtNode> &stmt) {}
