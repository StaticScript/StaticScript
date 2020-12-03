#include "Sema/TypeChecker.h"
#include <algorithm>

void TypeChecker::visit(const SharedPtr<VarDeclNode> &varDecl) {
    SharedPtr<Type> &varDeclType = varDecl->type;
    SharedPtr<ExprNode> &varInitVal = varDecl->initVal;
    if (varInitVal) {
        varInitVal->accept(shared_from_this());
        const SharedPtr<Type> &initValType = varInitVal->type;
        if (varDeclType) {
            if (!varDeclType->compatible(initValType)) {
                reportTypeError("The variable declaration type does not match the initial value type: " + varDecl->name);
            }
        } else if (!initValType) {
            reportTypeError("Unknown variable initialization type");
        } else {
            if (initValType->isArray() && initValType->isUnknown()) {
                reportTypeError(
                        "When an empty array literal is used as the initial value of a variable, the variable must explicitly specify the type"
                );
            }
            varDeclType = initValType;
        }
        if (varDeclType->isArray()) {
            // 把显式的声明类型递归赋回给初始值的类型
            const auto &initArrayLiteral = staticPtrCast<ArrayLiteralExprNode>(varInitVal);
            const auto &varArrayType = staticPtrCast<ArrayType>(varDeclType);
            assignTypeForArrayLiteral(initArrayLiteral, varArrayType);
        }
    } else {
        reportOnTypeError(!varDeclType, "Variable has no type specified: " + varDecl->name);
        // 如果变量声明时未指定初始值, 则默认为其类型对应的零值
        SharedPtr<ExprNode> initVal;
        if (varDeclType->isBoolean()) {
            initVal = makeShared<BooleanLiteralExprNode>(false);
        } else if (varDeclType->isInteger()) {
            initVal = makeShared<IntegerLiteralExprNode>(0);
        } else if (varDeclType->isFloat()) {
            initVal = makeShared<FloatLiteralExprNode>(0.0);
        } else if (varDeclType->isString()) {
            initVal = makeShared<StringLiteralExprNode>();
        } else if (varDeclType->isArray()) {
            initVal = makeShared<ArrayLiteralExprNode>(varDeclType);
        }
        varInitVal = initVal;
    }
}

void TypeChecker::visit(const SharedPtr<ArraySubscriptExprNode> &asExpr) {
    ASTVisitor::visit(asExpr);
    if (!asExpr->baseExpr->type->isArray()) {
        reportTypeError("Base of array subscript expression is not array type");
    }
    for (const SharedPtr<ExprNode> &indexExpr : asExpr->indexExprs) {
        if (!indexExpr->type->isInteger()) {
            reportTypeError(
                    "The subscript of an array subscript expression can only be a natural number. The current subscript is not of integer type");
        }
        if (auto intLiteral = dynPtrCast<IntegerLiteralExprNode>(indexExpr); intLiteral && intLiteral->literal < 0) {
            reportTypeError("The subscript of an array subscript expression can only be a natural number. The current subscript is less than 0");
        }
    }
    SharedPtr<Type> iterType = asExpr->baseExpr->type;
    for (size_t i = 0; i < asExpr->indexExprs.size(); ++i) {
        iterType = iterType->asArray()->getElementType();
    }
    asExpr->type = iterType;
}

void TypeChecker::visit(const SharedPtr<CallExprNode> &callExpr) {
    ASTVisitor::visit(callExpr);
    const SharedPtrVector<ExprNode> &args = callExpr->args;
    const SharedPtrVector<ParmVarDeclNode> &params = callExpr->refFuncDecl->params;
    size_t argsSize = args.size();
    size_t paramsSize = params.size();
    if (argsSize != paramsSize) {
        reportTypeError("The number of arguments did not match when calling '" + callExpr->calleeName + "' function");
    }
    for (size_t i = 0; i < argsSize; ++i) {
        if (!params[i]->type->compatible(args[i]->type)) {
            reportTypeError(
                    "The type of the " +
                    std::to_string(i + 1) +
                    "-th parameter does not match when calling '" +
                    callExpr->calleeName +
                    "' function"
            );
        }
    }
    callExpr->type = callExpr->refFuncDecl->returnType;
}

void TypeChecker::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);

    unsigned int uop = uopExpr->opCode;
    const SharedPtr<Type> &type = uopExpr->subExpr->type;

    switch (uop) {
        case StaticScriptLexer::Plus:
        case StaticScriptLexer::Minus:
        case StaticScriptLexer::PlusPlus:
        case StaticScriptLexer::MinusMinus: {
            if (!type->isNumber()) {
                reportTypeError("The current unary operator only allows operations on numbers");
            }
            break;
        }
        case StaticScriptLexer::Not: {
            // !运算符的算子必须是布尔类型
            if (!type->isBoolean()) {
                reportTypeError("Operator '!' allows only Boolean operations");
            }
            break;
        }
        case StaticScriptLexer::BitNot: {
            if (!type->isInteger()) {
                reportTypeError("The current unary operator only allows operations on integers");
            }
            break;
        }
        default:
            break;
    }
    uopExpr->type = type;
}

void TypeChecker::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    ASTVisitor::visit(bopExpr);

    unsigned int bop = bopExpr->opCode;
    const SharedPtr<Type> &leftType = bopExpr->lhs->type;
    const SharedPtr<Type> &rightType = bopExpr->rhs->type;

    // 二元运算符只支持基础类型
    if (!leftType->isBasic() || !rightType->isBasic()) {
        reportTypeError("Binary operators only supports basic types(boolean/integer/float/string");
    }

    // 所有的二元运算符只支持运算同类类型的表达式)
    if (!leftType->sameAs(rightType)) {
        reportTypeError("Binary operator '" + std::to_string(bop) + "' only supports expressions of the same type");
    }

    // 检查算数类型是否符合运算符要求
    if (bop == StaticScriptLexer::Plus || bop == StaticScriptLexer::PlusAssign) {
        if (!leftType->isNumber() &&
            !leftType->isString()) {
            reportTypeError("Add string and integer only operation types are supported");
        }
    } else if (bop >= StaticScriptLexer::Minus && bop <= StaticScriptLexer::Modulus ||
               bop >= StaticScriptLexer::MinusAssign && bop <= StaticScriptLexer::ModulusAssign ||
               bop >= StaticScriptLexer::LessThan && bop <= StaticScriptLexer::GreaterThanEquals) {
        if (!leftType->isNumber()) {
            reportTypeError("Binary operator '" + std::to_string(bop) + "' only supports number type");
        }
    } else if (
            bop >= StaticScriptLexer::BitAnd && bop <= StaticScriptLexer::LogicalShiftRight ||
            bop >= StaticScriptLexer::BitAndAssign && bop <= StaticScriptLexer::LogicalShiftRightAssign
            ) {
        if (!leftType->isInteger()) {
            reportTypeError("Bitwise operator '" + std::to_string(bop) + "' only supports integer type");
        }
    } else if (bop == StaticScriptLexer::And ||
               bop == StaticScriptLexer::Or ||
               bop == StaticScriptLexer::AndAssign ||
               bop == StaticScriptLexer::OrAssign) {
        if (!leftType->isBoolean()) {
            reportTypeError("Binary operator '" + std::to_string(bop) + "' only supports boolean type");
        }
    }

    // 设置二元运算表达式的类型
    if (bop >= StaticScriptLexer::LessThan && bop <= StaticScriptLexer::NotEquals) {
        bopExpr->type = BasicType::BOOLEAN_TYPE;
    } else if (leftType->isFloat() || rightType->isFloat()) {
        bopExpr->type = BasicType::FLOAT_TYPE;
    } else {
        bopExpr->type = leftType;
    }
}

void TypeChecker::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    varExpr->type = varExpr->refVarDecl->type;
}

void TypeChecker::visit(const SharedPtr<ArrayLiteralExprNode> &arrayLiteralExpr) {
    ASTVisitor::visit(arrayLiteralExpr);
    // 数组元素非空则寻找该数组的元素类型
    if (!arrayLiteralExpr->elements.empty()) {
        // 寻找非unknown的element
        auto primaryElement = std::find_if(
                arrayLiteralExpr->elements.begin(),
                arrayLiteralExpr->elements.end(),
                [](const SharedPtr<ExprNode> &element) {
                    return !element->type->isUnknown();
                }
        );
        SharedPtr<Type> primaryElementType;
        // 如果找到了非unknown的element
        if (primaryElement != arrayLiteralExpr->elements.end()) {
            primaryElementType = (*primaryElement)->type;
            for (const SharedPtr<ExprNode> &element : arrayLiteralExpr->elements) {
                if (!primaryElementType->compatible(element->type)) {
                    reportTypeError("Inconsistent array element type");
                } else if (element->type->isFloat()) {
                    // 如果数组中有一个元素为浮点型, 那么所有的元素都设置为浮点型
                    primaryElementType = BasicType::FLOAT_TYPE;
                }
            }
        } else {
            // 所有元素类型都是unknown, 即多维数组
            primaryElementType = arrayLiteralExpr->elements[0]->type;
            // 判断多维数组的子数组深度是否一致
            for (const SharedPtr<ExprNode> &element : arrayLiteralExpr->elements) {
                if (primaryElementType->asArray()->getDepth() != element->type->asArray()->getDepth()) {
                    reportTypeError("Inconsistent depth of multidimensional empty array elements");
                }
            }
        }
        arrayLiteralExpr->type = makeShared<ArrayType>(primaryElementType);
    } else {
        // 空数组则默认为其元素类型为unknown
        arrayLiteralExpr->type = makeShared<ArrayType>();
    }
}

void TypeChecker::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ASTVisitor::visit(ifStmt);
    if (!ifStmt->condition->type->isBoolean()) {
        reportTypeError("The condition of if statement only supports boolean type");
    }
}

void TypeChecker::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    ASTVisitor::visit(whileStmt);
    if (!whileStmt->condition->type->isBoolean()) {
        reportTypeError("The condition of while statement only supports boolean type temporarily");
    }
}

void TypeChecker::visit(const SharedPtr<ForStmtNode> &forStmt) {
    ASTVisitor::visit(forStmt);
    if (forStmt->condition && !forStmt->condition->type->isBoolean()) {
        reportTypeError("The condition of for statement only supports boolean type temporarily");
    }
}

void TypeChecker::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
    const auto &returnExpr = returnStmt->returnExpr;
    auto &returnType = returnStmt->refFuncDecl->returnType;
    if (returnExpr) {
        if (!returnType) {
            returnType = returnExpr->type;
        } else if (!returnExpr->type->sameAs(returnType)) {
            reportTypeError("The return value type does not match the function return value type");
        }
    } else if (returnType) {
        reportTypeError("The return statement is missing a return value");
    }
}

void TypeChecker::assignTypeForArrayLiteral(const SharedPtr<ArrayLiteralExprNode> &arrayLiteral, const SharedPtr<ArrayType> &type) {
    arrayLiteral->type = type;
    for (const SharedPtr<ExprNode> &itemExpr : arrayLiteral->elements) {
        const SharedPtr<Type> &elementType = type->getElementType();
        if (const auto &subArrayLiteral = dynPtrCast<ArrayLiteralExprNode>(itemExpr)) {
            assignTypeForArrayLiteral(subArrayLiteral, elementType->asArray());
        } else {
            itemExpr->type = elementType;
        }
    }
}
