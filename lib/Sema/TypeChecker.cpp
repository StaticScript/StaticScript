#include "Sema/TypeChecker.h"

void TypeChecker::visit(const SharedPtr<VarDeclNode> &varDecl) {
    if (varDecl->initVal) {
        varDecl->initVal->accept(shared_from_this());
        if (varDecl->type) {
            if (varDecl->type != varDecl->initVal->inferType) {
                throw SemanticException("变量声明类型与初始值类型不匹配: " + varDecl->name);
            }
        } else {
            varDecl->type = varDecl->initVal->inferType;
        }
    } else {
        if (!varDecl->type) {
            throw SemanticException("变量未指定类型: " + varDecl->name);
        }
        // 如果变量声明时未指定初始值, 则默认为其类型对应的零值
        if (varDecl->type == BuiltinTypeNode::BOOLEAN_TYPE) {
            varDecl->initVal = makeShared<BooleanLiteralExprNode>(false);
        } else if (varDecl->type == BuiltinTypeNode::INTEGER_TYPE) {
            varDecl->initVal = makeShared<IntegerLiteralExprNode>(0);
        } else {
            varDecl->initVal = makeShared<StringLiteralExprNode>();
        }
    }
}

void TypeChecker::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    varExpr->inferType = varExpr->refVarDecl->type;
}

void TypeChecker::visit(const SharedPtr<CallExprNode> &callExpr) {
    ASTVisitor::visit(callExpr);
    size_t argsSize = callExpr->args.size();
    size_t paramsSize = callExpr->refFuncDecl->params.size();
    if (argsSize != paramsSize) {
        throw SemanticException("调用" + callExpr->calleeName + "函数时参数数量不匹配");
    }
    for (size_t i = 0; i < argsSize; ++i) {
        if (callExpr->args[i]->inferType != callExpr->refFuncDecl->params[i]->type) {
            throw SemanticException("调用" + callExpr->calleeName + "函数时第" + std::to_string(i + 1) + "个参数类型不匹配");
        }
    }
    callExpr->inferType = callExpr->refFuncDecl->returnType;
}

void TypeChecker::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);
    switch (uopExpr->opCode) {
        case StaticScriptLexer::Minus: {
            if (uopExpr->subExpr->inferType != BuiltinTypeNode::INTEGER_TYPE) {
                throw SemanticException("目前Minus运算符只允许对整数进行运算");
            }
            break;
        }
        default: {
            throw SemanticException("未知的一元运算符");
        }
    }
    uopExpr->inferType = uopExpr->subExpr->inferType;
}

void TypeChecker::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);
    const SharedPtr<BuiltinTypeNode> &leftType = bopExpr->lhs->inferType;
    const SharedPtr<BuiltinTypeNode> &rightType = bopExpr->rhs->inferType;
    // 检查二元运算符的运算表达式是否符合类型要求
    switch (bopExpr->opCode) {
        case StaticScriptLexer::Minus:
        case StaticScriptLexer::Multiply:
        case StaticScriptLexer::Divide:
        case StaticScriptLexer::LessThan:
        case StaticScriptLexer::LessThanEquals:
        case StaticScriptLexer::GreaterThan:
        case StaticScriptLexer::GreaterThanEquals: {
            if (leftType != BuiltinTypeNode::INTEGER_TYPE) {
                throw SemanticException("二元运算符" + std::to_string(bopExpr->opCode) + "只支持整数类型");
            }
            if (rightType != BuiltinTypeNode::INTEGER_TYPE) {
                throw SemanticException("二元运算符" + std::to_string(bopExpr->opCode) + "只支持整数类型");
            }
            break;
        }
        case StaticScriptLexer::Plus: {
            if (leftType != rightType) {
                throw SemanticException("二元运算符+只支持运算同类型的表达式");
            }
            if (leftType != BuiltinTypeNode::INTEGER_TYPE &&
                leftType != BuiltinTypeNode::STRING_TYPE) {
                throw SemanticException("+运算符只支持运算整数类型和字符串类型");
            }
            break;
        }
        case StaticScriptLexer::Assign:
        case StaticScriptLexer::Equals:
        case StaticScriptLexer::NotEquals: {
            if (leftType != rightType) {
                throw SemanticException("二元运算符" + std::to_string(bopExpr->opCode) + "只支持运算同类型的表达式");
            }
            break;
        }
        default: {
            throw SemanticException("未知的二元运算符");
        }
    }
    // 设置二元运算表达式的类型
    switch (bopExpr->opCode) {
        case StaticScriptLexer::LessThan:
        case StaticScriptLexer::LessThanEquals:
        case StaticScriptLexer::GreaterThan:
        case StaticScriptLexer::GreaterThanEquals:
        case StaticScriptLexer::Equals:
        case StaticScriptLexer::NotEquals: {
            bopExpr->inferType = BuiltinTypeNode::BOOLEAN_TYPE;
            break;
        }
        case StaticScriptLexer::Plus:
        case StaticScriptLexer::Minus:
        case StaticScriptLexer::Multiply:
        case StaticScriptLexer::Divide:
        case StaticScriptLexer::Assign: {
            bopExpr->inferType = leftType;
            break;
        }
    }
}

void TypeChecker::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ASTVisitor::visit(ifStmt);
    if (ifStmt->condition->inferType != BuiltinTypeNode::BOOLEAN_TYPE) {
        throw SemanticException("if语句条件暂时只支持boolean类型");
    }
}

void TypeChecker::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    ASTVisitor::visit(whileStmt);
    if (whileStmt->condition->inferType != BuiltinTypeNode::BOOLEAN_TYPE) {
        throw SemanticException("while语句条件暂时只支持boolean类型");
    }
}

void TypeChecker::visit(const SharedPtr<ForStmtNode> &forStmt) {
    ASTVisitor::visit(forStmt);
    if (forStmt->condition &&
        forStmt->condition->inferType != BuiltinTypeNode::BOOLEAN_TYPE) {
        throw SemanticException("for语句条件暂时只支持boolean类型");
    }
}

void TypeChecker::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
    if (returnStmt->returnExpr) {
        if (returnStmt->returnExpr->inferType != returnStmt->refFuncDecl->returnType) {
            throw SemanticException("return返回值类型与函数返回值类型不匹配");
        }
    }
}
