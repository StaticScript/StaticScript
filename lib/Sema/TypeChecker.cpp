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
        if (varDecl->isConstant()) {
            throw SemanticException("const常量必须有初始值");
        }
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

    unsigned int uop = uopExpr->opCode;
    const SharedPtr<BuiltinTypeNode> &inferType = uopExpr->subExpr->inferType;

    // 分别检查!运算符和其他运算符的算子类型
    if (uop == StaticScriptLexer::Not) {
        // !运算符的算子必须是布尔类型
        if (inferType != BuiltinTypeNode::BOOLEAN_TYPE) {
            throw SemanticException("!运算符只允许对布尔值进行运算");
        }
    } else {
        // 其他运算符的算子必须是整数类型
        if (inferType != BuiltinTypeNode::INTEGER_TYPE) {
            throw SemanticException("当前一元运算符只允许对整数进行运算");
        }
    }
    uopExpr->inferType = inferType;
}

void TypeChecker::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);

    unsigned int bop = bopExpr->opCode;
    const SharedPtr<BuiltinTypeNode> &leftInferType = bopExpr->lhs->inferType;
    const SharedPtr<BuiltinTypeNode> &rightInferType = bopExpr->rhs->inferType;

    // 所有的二元运算符只支持运算同类型的表达式
    if (leftInferType != rightInferType) {
        throw SemanticException("二元运算符" + std::to_string(bop) + "只支持运算同类型的表达式");
    }

    if (bop == StaticScriptLexer::Plus || bop == StaticScriptLexer::PlusAssign) {
        if (leftInferType != BuiltinTypeNode::INTEGER_TYPE &&
            leftInferType != BuiltinTypeNode::STRING_TYPE) {
            throw SemanticException("加运算只支持整数类型和字符串类型");
        }
    } else if (bop >= StaticScriptLexer::Minus && bop <= StaticScriptLexer::GreaterThanEquals ||
               bop >= StaticScriptLexer::MinusAssign && bop <= StaticScriptLexer::RightShiftAssign) {
        if (leftInferType != BuiltinTypeNode::INTEGER_TYPE) {
            throw SemanticException("二元运算符" + std::to_string(bop) + "只支持整数类型");
        }
    } else if (bop == StaticScriptLexer::And ||
               bop == StaticScriptLexer::Or ||
               bop == StaticScriptLexer::AndAssign ||
               bop == StaticScriptLexer::OrAssign) {
        if (leftInferType != BuiltinTypeNode::BOOLEAN_TYPE) {
            throw SemanticException("二元运算符" + std::to_string(bop) + "只支持布尔类型");
        }
    }

    // 设置二元运算表达式的类型
    if (bop >= StaticScriptLexer::LessThan && bop <= StaticScriptLexer::NotEquals) {
        bopExpr->inferType = BuiltinTypeNode::BOOLEAN_TYPE;
    } else {
        bopExpr->inferType = leftInferType;
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
    } else {
        if (returnStmt->refFuncDecl->returnType) {
            throw SemanticException("return语句缺少返回值");
        }
    }
}
