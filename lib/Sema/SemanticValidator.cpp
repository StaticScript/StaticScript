#include "Sema/SemanticValidator.h"

void SemanticValidator::visit(const SharedPtr<ModuleNode> &module) {
    ASTVisitor::visit(module);
}

void SemanticValidator::visit(const SharedPtr<BuiltinTypeNode> &builtinType) {
    ASTVisitor::visit(builtinType);
}

void SemanticValidator::visit(const SharedPtr<VarDeclNode> &varDecl) {
    ASTVisitor::visit(varDecl);
}

void SemanticValidator::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    ASTVisitor::visit(paramVarDecl);
}

void SemanticValidator::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    ASTVisitor::visit(funcDecl);
    if (funcDecl->returnType) {
        if (!funcDecl->refReturnStmt) {
            throw SemanticException("有返回类型的函数必须有return语句");
        }
    }
}

void SemanticValidator::visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr) {
    ASTVisitor::visit(boolLiteralExpr);
}

void SemanticValidator::visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr) {
    ASTVisitor::visit(intLiteralExpr);
}

void SemanticValidator::visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) {
    ASTVisitor::visit(strLiteralExpr);
}

void SemanticValidator::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    ASTVisitor::visit(varExpr);
}

void SemanticValidator::visit(const SharedPtr<CallExprNode> &callExpr) {
    ASTVisitor::visit(callExpr);
}

void SemanticValidator::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);
}

void SemanticValidator::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);
    if (bopExpr->opCode == StaticScriptLexer::Assign) {
        SharedPtr<IdentifierExprNode> varExpr = dynPtrCast<IdentifierExprNode>(bopExpr->lhs);
        if (varExpr) {
            if (varExpr->refVarDecl->modifier == VarModifier::Const) {
                throw SemanticException("不允许对常量赋值");
            }
        } else {
            throw SemanticException("不允许对右值赋值");
        }
    }
}

void SemanticValidator::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    ASTVisitor::visit(exprStmt);
}

void SemanticValidator::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    ASTVisitor::visit(compStmt);
}

void SemanticValidator::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    ASTVisitor::visit(varDeclStmt);
}

void SemanticValidator::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    ASTVisitor::visit(funcDeclStmt);
}

void SemanticValidator::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ASTVisitor::visit(ifStmt);
}

void SemanticValidator::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    ASTVisitor::visit(whileStmt);
}

void SemanticValidator::visit(const SharedPtr<ForStmtNode> &forStmt) {
    ASTVisitor::visit(forStmt);
}

void SemanticValidator::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {
    if (!isInLoop(continueStmt)) {
        throw SemanticException("continue语句语句只能出现在循环里");
    }
}

void SemanticValidator::visit(const SharedPtr<BreakStmtNode> &breakStmt) {
    if (!isInLoop(breakStmt)) {
        throw SemanticException("break语句只能出现在循环里");
    }
}

void SemanticValidator::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
}

bool SemanticValidator::isInLoop(const SharedPtr<Node> &node) {
    SharedPtr<Node> iterNode = node->parent;
    SharedPtr<WhileStmtNode> ancestorWhileStmt;
    SharedPtr<ForStmtNode> ancestorForStmt;
    while (iterNode) {
        SharedPtr<WhileStmtNode> whileStmt = dynPtrCast<WhileStmtNode>(iterNode);
        if (whileStmt) {
            ancestorWhileStmt = whileStmt;
            break;
        }
        SharedPtr<ForStmtNode> forStmt = dynPtrCast<ForStmtNode>(iterNode);
        if (forStmt) {
            ancestorForStmt = forStmt;
            break;
        }
        iterNode = iterNode->parent;
    }
    return ancestorWhileStmt || ancestorForStmt;
}
