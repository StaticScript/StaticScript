#include "CodeGen/IRGenerator.h"

void IRGenerator::visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr) {
    boolLiteralExpr->code = boolLiteralExpr->literal ?
                            llvmIRBuilder.getTrue() :
                            llvmIRBuilder.getFalse();
}

void IRGenerator::visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr) {
    intLiteralExpr->code = LLVMConstantInt::get(
            llvmIRBuilder.getInt64Ty(),
            intLiteralExpr->literal,
            true
    );
}

void IRGenerator::visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) {
    llvm::Constant *literal = llvmIRBuilder.CreateGlobalString(strLiteralExpr->literal);
    llvm::Value *argLiteral = llvmIRBuilder.CreatePointerCast(literal, llvmIRBuilder.getInt8PtrTy());
    strLiteralExpr->code = llvmIRBuilder.CreateCall(BuiltinString::createFunc, argLiteral);
}

void IRGenerator::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    // 按右值处理
    varExpr->code = llvmIRBuilder.CreateLoad(varExpr->refVarDecl->code);
}

void IRGenerator::visit(const SharedPtr<CallExprNode> &callExpr) {
    ASTVisitor::visit(callExpr);
    llvm::Function *calleeFunc = llvmModule->getFunction(callExpr->calleeName);
    if (!calleeFunc) {
        throw CodeGenException("没有找到函数");
    }
    if (calleeFunc->arg_size() != callExpr->args.size()) {
        throw CodeGenException("函数参数传递不正确");
    }
    Vector<LLVMValue *> argsV;
    for (size_t i = 0, e = callExpr->args.size(); i != e; ++i) {
        argsV.push_back(callExpr->args[i]->code);
    }
    callExpr->code = llvmIRBuilder.CreateCall(calleeFunc, argsV);
}

void IRGenerator::visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) {
    ASTVisitor::visit(uopExpr);

    SharedPtr<ExprNode> subExpr = uopExpr->subExpr;

    switch (uopExpr->opCode) {
        case StaticScriptLexer::PlusPlus:
        case StaticScriptLexer::MinusMinus: {
            const SharedPtr<IdentifierExprNode> &subVarExpr = staticPtrCast<IdentifierExprNode>(subExpr);
            LLVMValue *value = llvmIRBuilder.CreateLoad(subVarExpr->refVarDecl->code);
            LLVMValue *newValue;
            LLVMConstantInt *one = llvmIRBuilder.getInt64(1);

            if (uopExpr->opCode == StaticScriptLexer::PlusPlus) {
                newValue = llvmIRBuilder.CreateNSWAdd(value, one);
            } else {
                newValue = llvmIRBuilder.CreateNSWSub(value, one);
            }
            if (uopExpr->isPostfix) {
                llvmIRBuilder.CreateStore(newValue, subVarExpr->refVarDecl->code);
                uopExpr->code = value;
            } else {
                llvmIRBuilder.CreateStore(newValue, subVarExpr->refVarDecl->code);
                uopExpr->code = newValue;
            }
            break;
        }
        case StaticScriptLexer::Not: {
            uopExpr->code = llvmIRBuilder.CreateXor(subExpr->code, llvmIRBuilder.getTrue());
            break;
        }
        case StaticScriptLexer::BitNot: {
            uopExpr->code = llvmIRBuilder.CreateXor(subExpr->code, llvmIRBuilder.getInt64(-1));
            break;
        }
        case StaticScriptLexer::Plus: {
            uopExpr->code = subExpr->code;
            break;
        }
        case StaticScriptLexer::Minus: {
            uopExpr->code = llvmIRBuilder.CreateNSWNeg(subExpr->code);
            break;
        }
    }
}

void IRGenerator::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    unsigned int bopCode = bopExpr->opCode;

    if (bopCode == StaticScriptLexer::Assign) {
        bopExpr->rhs->accept(shared_from_this());
    } else {
        ASTVisitor::visit(bopExpr);
    }

    const SharedPtr<BuiltinTypeNode> &type = bopExpr->lhs->inferType;
    LLVMValue *lhsCode = bopExpr->lhs->code;
    LLVMValue *rhsCode = bopExpr->rhs->code;
    LLVMValue *targetCode = nullptr;

    switch (bopCode) {
        case StaticScriptLexer::Plus:
        case StaticScriptLexer::PlusAssign: {
            if (type == BuiltinTypeNode::STRING_TYPE) {
                Vector<LLVMValue *> argsV{lhsCode, rhsCode};
                targetCode = llvmIRBuilder.CreateCall(BuiltinString::concatFunc, argsV);
            } else if (type == BuiltinTypeNode::INTEGER_TYPE) {
                targetCode = llvmIRBuilder.CreateNSWAdd(lhsCode, rhsCode);
            }
            break;
        }
        case StaticScriptLexer::Minus:
        case StaticScriptLexer::MinusAssign: {
            targetCode = llvmIRBuilder.CreateNSWSub(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::Multiply:
        case StaticScriptLexer::MultiplyAssign: {
            targetCode = llvmIRBuilder.CreateNSWMul(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::Divide:
        case StaticScriptLexer::DivideAssign: {
            targetCode = llvmIRBuilder.CreateSDiv(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::Modulus:
        case StaticScriptLexer::ModulusAssign: {
            targetCode = llvmIRBuilder.CreateSRem(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::BitAnd:
        case StaticScriptLexer::BitAndAssign: {
            targetCode = llvmIRBuilder.CreateAnd(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::BitXOr:
        case StaticScriptLexer::BitXorAssign: {
            targetCode = llvmIRBuilder.CreateXor(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::BitOr:
        case StaticScriptLexer::BitOrAssign: {
            targetCode = llvmIRBuilder.CreateOr(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::LeftShift:
        case StaticScriptLexer::LeftShiftAssign: {
            targetCode = llvmIRBuilder.CreateShl(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::RightShift:
        case StaticScriptLexer::RightShiftAssign: {
            targetCode = llvmIRBuilder.CreateAShr(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::And:
        case StaticScriptLexer::AndAssign: {
            targetCode = llvmIRBuilder.CreateAnd(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::Or:
        case StaticScriptLexer::OrAssign: {
            targetCode = llvmIRBuilder.CreateOr(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::LessThan: {
            targetCode = llvmIRBuilder.CreateICmpSLT(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::GreaterThan: {
            targetCode = llvmIRBuilder.CreateICmpSGT(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::LessThanEquals: {
            targetCode = llvmIRBuilder.CreateICmpSLE(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::GreaterThanEquals: {
            targetCode = llvmIRBuilder.CreateICmpSGE(lhsCode, rhsCode);
            break;
        }
        case StaticScriptLexer::Equals: {
            if (type == BuiltinTypeNode::STRING_TYPE) {
                Vector<LLVMValue *> argsV{lhsCode, rhsCode};
                LLVMValue *relationship = llvmIRBuilder.CreateCall(BuiltinString::equalsFunc, argsV);
                targetCode = llvmIRBuilder.CreateICmpEQ(relationship, llvmIRBuilder.getInt64(0));
            } else {
                targetCode = llvmIRBuilder.CreateICmpEQ(lhsCode, rhsCode);
            }
            break;
        }
        case StaticScriptLexer::NotEquals: {
            if (type == BuiltinTypeNode::STRING_TYPE) {
                Vector<LLVMValue *> argsV{lhsCode, rhsCode};
                LLVMValue *relationship = llvmIRBuilder.CreateCall(BuiltinString::equalsFunc, argsV);
                targetCode = llvmIRBuilder.CreateICmpNE(relationship, llvmIRBuilder.getInt64(0));
            } else {
                targetCode = llvmIRBuilder.CreateICmpNE(lhsCode, rhsCode);
            }
            break;
        }
        default:
            break;
    }
    if (bopCode >= StaticScriptLexer::Assign && bopCode <= StaticScriptLexer::OrAssign) {
        // 在语义阶段保证lhs类型为IdentifierExprNode
        // 按左值处理
        SharedPtr<IdentifierExprNode> leftVarExpr = staticPtrCast<IdentifierExprNode>(bopExpr->lhs);
        if (bopCode == StaticScriptLexer::Assign) {
            llvmIRBuilder.CreateStore(rhsCode, leftVarExpr->refVarDecl->code);
            targetCode = llvmIRBuilder.CreateLoad(leftVarExpr->refVarDecl->code);
        } else {
            llvmIRBuilder.CreateStore(targetCode, leftVarExpr->refVarDecl->code);
        }
    }
    bopExpr->code = targetCode;
}