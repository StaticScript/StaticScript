#include "Sema/TypeChecker.h"
#include <algorithm>

void TypeChecker::visit(const SharedPtr<VarDeclNode> &varDecl) {
    if (varDecl->initVal) {
        varDecl->initVal->accept(shared_from_this());
        SharedPtr<Type> &initValInferType = varDecl->initVal->inferType;
        if (varDecl->type) {
            if (!varDecl->type->equals(initValInferType)) {
                if (varDecl->type->isArray() && initValInferType->isArray() && initValInferType->isUnknown()) {
                } else {
                    reportTypeError("The variable declaration type does not match the initial value type: " + varDecl->name);
                }
            }
        } else if (!initValInferType) {
            reportTypeError("Unknown variable initialization type");
        } else {
            if (initValInferType->isArray() && initValInferType->isUnknown()) {
                reportTypeError(
                        "When an empty array literal is used as the initial value of a variable, the variable must explicitly specify the type");
            }
            varDecl->type = initValInferType;
        }
        if (varDecl->type->isArray()) {
            // 把显式的声明类型递归赋回给初始值的类型
            assignTypeForArrayLiteral(staticPtrCast<ArrayLiteralExprNode>(varDecl->initVal), staticPtrCast<ArrayType>(varDecl->type));
        }
    } else {
        reportOnTypeError(!varDecl->type, "Variable has no type specified: " + varDecl->name);
        // 如果变量声明时未指定初始值, 则默认为其类型对应的零值
        if (varDecl->type->isAtomic()) {
            if (varDecl->type->equals(AtomicType::BOOLEAN_TYPE)) {
                varDecl->initVal = makeShared<BooleanLiteralExprNode>(false);
            } else if (varDecl->type->equals(AtomicType::INTEGER_TYPE)) {
                varDecl->initVal = makeShared<IntegerLiteralExprNode>(0);
            } else if (varDecl->type->equals(AtomicType::STRING_TYPE)) {
                varDecl->initVal = makeShared<StringLiteralExprNode>();
            }
        } else if (varDecl->type->isArray()) {
            varDecl->initVal = makeShared<ArrayLiteralExprNode>(varDecl->type);
        }
    }
}

void TypeChecker::visit(const SharedPtr<ArraySubscriptExprNode> &asExpr) {
    ASTVisitor::visit(asExpr);
    if (!asExpr->baseExpr->inferType->isArray()) {
        reportTypeError("Base of array subscript expression is not array type");
    }
    for (const SharedPtr<ExprNode> &indexExpr : asExpr->indexExprs) {
        if (!indexExpr->inferType->equals(AtomicType::INTEGER_TYPE)) {
            reportTypeError(
                    "The subscript of an array subscript expression can only be a natural number. The current subscript is not of integer type");
        }
        if (auto intLiteral = dynPtrCast<IntegerLiteralExprNode>(indexExpr); intLiteral && intLiteral->literal < 0) {
            reportTypeError("The subscript of an array subscript expression can only be a natural number. The current subscript is less than 0");
        }
    }
    SharedPtr<Type> iterType = asExpr->baseExpr->inferType;
    for (size_t i = 0; i < asExpr->indexExprs.size(); ++i) {
        iterType = staticPtrCast<ArrayType>(iterType)->getElementType();
    }
    asExpr->inferType = iterType;
}

void TypeChecker::visit(const SharedPtr<CallExprNode> &callExpr) {
    ASTVisitor::visit(callExpr);
    size_t argsSize = callExpr->args.size();
    size_t paramsSize = callExpr->refFuncDecl->params.size();
    if (argsSize != paramsSize) {
        reportTypeError("The number of arguments did not match when calling '" + callExpr->calleeName + "' function");
    }
    for (size_t i = 0; i < argsSize; ++i) {
        if (callExpr->args[i]->inferType != callExpr->refFuncDecl->params[i]->type) {
            reportTypeError(
                    "The type of the " +
                    std::to_string(i + 1) +
                    "-th parameter does not match when calling '" +
                    callExpr->calleeName +
                    "' function");
        }
    }
    callExpr->inferType = callExpr->refFuncDecl->returnType;
}

void TypeChecker::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);

    unsigned int uop = uopExpr->opCode;
    const SharedPtr<Type> &inferType = uopExpr->subExpr->inferType;

    // 分别检查!运算符和其他运算符的算子类型
    if (uop == StaticScriptLexer::Not) {
        // !运算符的算子必须是布尔类型
        if (!inferType->equals(AtomicType::BOOLEAN_TYPE)) {
            reportTypeError("Operator '!' allows only Boolean operations");
        }
    } else {
        // 其他运算符的算子必须是整数类型
        if (!inferType->equals(AtomicType::INTEGER_TYPE)) {
            reportTypeError("The current unary operator only allows operations on integers");
        }
    }
    uopExpr->inferType = inferType;
}

void TypeChecker::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);

    unsigned int bop = bopExpr->opCode;
    const SharedPtr<Type> &leftInferType = bopExpr->lhs->inferType;
    const SharedPtr<Type> &rightInferType = bopExpr->rhs->inferType;

    // 所有的二元运算符只支持运算同类型的表达式
    if (leftInferType != rightInferType) {
        reportTypeError("Binary operator '" + std::to_string(bop) + "' only supports expressions of the same type");
    }

    if (bop == StaticScriptLexer::Plus || bop == StaticScriptLexer::PlusAssign) {
        if (!leftInferType->equals(AtomicType::INTEGER_TYPE) &&
            !leftInferType->equals(AtomicType::STRING_TYPE)) {
            reportTypeError("Add string and integer only operation types are supported");
        }
    } else if (bop >= StaticScriptLexer::Minus && bop <= StaticScriptLexer::GreaterThanEquals ||
               bop >= StaticScriptLexer::MinusAssign && bop <= StaticScriptLexer::RightShiftAssign) {
        if (!leftInferType->equals(AtomicType::INTEGER_TYPE)) {
            reportTypeError("Binary operator '" + std::to_string(bop) + "' only supports integer type");
        }
    } else if (bop == StaticScriptLexer::And ||
               bop == StaticScriptLexer::Or ||
               bop == StaticScriptLexer::AndAssign ||
               bop == StaticScriptLexer::OrAssign) {
        if (!leftInferType->equals(AtomicType::BOOLEAN_TYPE)) {
            reportTypeError("Binary operator '" + std::to_string(bop) + "' only supports boolean type");
        }
    }

    // 设置二元运算表达式的类型
    if (bop >= StaticScriptLexer::LessThan && bop <= StaticScriptLexer::NotEquals) {
        bopExpr->inferType = AtomicType::BOOLEAN_TYPE;
    } else {
        bopExpr->inferType = leftInferType;
    }
}

void TypeChecker::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    varExpr->inferType = varExpr->refVarDecl->type;
}

void TypeChecker::visit(const SharedPtr<ArrayLiteralExprNode> &arrayLiteralExpr) {
    ASTVisitor::visit(arrayLiteralExpr);
    if (!arrayLiteralExpr->elements.empty()) {
        // 寻找非unknown的element
        auto element = std::find_if(arrayLiteralExpr->elements.begin(), arrayLiteralExpr->elements.end(), [](const SharedPtr<ExprNode> &element) {
            return !element->inferType->isUnknown();
        });
        SharedPtr<Type> elementType;
        if (element != arrayLiteralExpr->elements.end()) {
            elementType = (*element)->inferType;
            for (const SharedPtr<ExprNode> &expr : arrayLiteralExpr->elements) {
                if (!elementType->equals(expr->inferType)) {
                    if (elementType->isArray() && expr->inferType->isArray() && expr->inferType->isUnknown()) {
                        if (staticPtrCast<ArrayType>(elementType)->getDepth() != staticPtrCast<ArrayType>(expr->inferType)->getDepth()) {
                            reportTypeError("Inconsistent depth of multidimensional array elements");
                        }
                    } else {
                        reportTypeError("Inconsistent array element type");
                    }
                }
            }
        } else {
            elementType = arrayLiteralExpr->elements[0]->inferType;
            for (const SharedPtr<ExprNode> &expr : arrayLiteralExpr->elements) {
                if (staticPtrCast<ArrayType>(elementType)->getDepth() != staticPtrCast<ArrayType>(expr->inferType)->getDepth()) {
                    reportTypeError("Inconsistent depth of multidimensional array elements");
                }
            }
        }
        arrayLiteralExpr->inferType = makeShared<ArrayType>(elementType);
    } else {
        arrayLiteralExpr->inferType = makeShared<ArrayType>();
    }
}

void TypeChecker::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ASTVisitor::visit(ifStmt);
    if (!ifStmt->condition->inferType->equals(AtomicType::BOOLEAN_TYPE)) {
        reportTypeError("The condition of if statement only supports boolean type");
    }
}

void TypeChecker::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    ASTVisitor::visit(whileStmt);
    if (!whileStmt->condition->inferType->equals(AtomicType::BOOLEAN_TYPE)) {
        reportTypeError("The condition of while statement only supports boolean type temporarily");
    }
}

void TypeChecker::visit(const SharedPtr<ForStmtNode> &forStmt) {
    ASTVisitor::visit(forStmt);
    if (forStmt->condition && !forStmt->condition->inferType->equals(AtomicType::BOOLEAN_TYPE)) {
        reportTypeError("The condition of for statement only supports boolean type temporarily");
    }
}

void TypeChecker::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
    if (returnStmt->returnExpr) {
        if (!returnStmt->returnExpr->inferType->equals(returnStmt->refFuncDecl->returnType)) {
            reportTypeError("The return value type does not match the function return value type");
        }
    } else {
        if (returnStmt->refFuncDecl->returnType) {
            reportTypeError("The return statement is missing a return value");
        }
    }
}

void TypeChecker::assignTypeForArrayLiteral(const SharedPtr<ArrayLiteralExprNode> &arrayLiteral, const SharedPtr<ArrayType> &type) {
    arrayLiteral->inferType = type;
    for (const SharedPtr<ExprNode> &itemExpr : arrayLiteral->elements) {
        if (const SharedPtr<ArrayLiteralExprNode> &subArrayLiteral = dynPtrCast<ArrayLiteralExprNode>(itemExpr)) {
            assignTypeForArrayLiteral(subArrayLiteral, staticPtrCast<ArrayType>(type->getElementType()));
        } else {
            itemExpr->inferType = type->getElementType();
        }
    }
}
