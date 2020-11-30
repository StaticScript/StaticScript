#include "Sema/SemanticValidator.h"

void SemanticValidator::visit(const SharedPtr<VarDeclNode> &varDecl) {
    ASTVisitor::visit(varDecl);
    if (varDecl->isConstant()) {
        reportOnSemanticError(!varDecl->initVal, "Constant must have an initial value");
        if (!dynPtrCast<LiteralExprNode>(varDecl->initVal)) {
            reportSemanticError("Constant initializers only support literal quantities");
        }
    }
}

void SemanticValidator::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    ASTVisitor::visit(funcDecl);
    if (funcDecl->returnType) {
        reportOnSemanticError(
                !funcDecl->refReturnStmt,
                "A function with a return type must have a return statement"
        );
    } else if (funcDecl->name == "main") {
        reportSemanticError("Function name cannot be 'main'");
    }
}

void SemanticValidator::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);
    // 只能对变量进行自增自减
    if (uopExpr->opCode == StaticScriptLexer::PlusPlus || uopExpr->opCode == StaticScriptLexer::MinusMinus) {
        if (!dynPtrCast<IdentifierExprNode>(uopExpr->subExpr) &&
            !dynPtrCast<ArraySubscriptExprNode>(uopExpr->subExpr)) {
            reportSemanticError("The increment and decrement operator only allows operations on variables or array elements");
        }
    }
}

void SemanticValidator::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);
    if (bopExpr->opCode >= StaticScriptLexer::Assign && bopExpr->opCode <= StaticScriptLexer::OrAssign) {
        if (const auto &varExpr = dynPtrCast<IdentifierExprNode>(bopExpr->lhs)) {
            if (varExpr->refVarDecl->isConstant()) {
                reportSemanticError("Assignment to a constant is not allowed");
            }
        } else if (const auto &asExpr = dynPtrCast<ArraySubscriptExprNode>(bopExpr->lhs)) {
        } else {
            reportSemanticError("Only variables or array elements are allowed to be assigned rather than expressions");
        }
    }
}

void SemanticValidator::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {
    if (!continueStmt->refIterationStmt) {
        reportSemanticError("A continue statement can only appear in a loop");
    }
}

void SemanticValidator::visit(const SharedPtr<BreakStmtNode> &breakStmt) {
    if (!breakStmt->refIterationStmt) {
        reportSemanticError("A break statement can only appear in a loop");
    }
}
