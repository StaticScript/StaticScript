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

void IRGenerator::visit(const SharedPtr<ArrayLiteralExprNode> &arrayLiteralExpr) {
    SharedPtr<ArrayType> arrayType = staticPtrCast<ArrayType>(arrayLiteralExpr->inferType);
    SharedPtr<Type> elementType = arrayType->getElementType();

    LLVMValue *errorAlloca = llvmIRBuilder.CreateAlloca(BuiltinError::type);
    llvmIRBuilder.CreateStore(llvm::ConstantPointerNull::get(BuiltinError::type), errorAlloca);
    if (arrayLiteralExpr->elements.empty()) {
        LLVMFunction *createFunc = nullptr;
        if (elementType->equals(AtomicType::BOOLEAN_TYPE)) {
            createFunc = BuiltinArray::createBooleanArrayFunc;
        } else if (elementType->equals(AtomicType::INTEGER_TYPE)) {
            createFunc = BuiltinArray::createIntegerArrayFunc;
        } else if (elementType->equals(AtomicType::STRING_TYPE)) {
            createFunc = BuiltinArray::createStringArrayFunc;
        } else if (elementType->isArray()) {
            createFunc = BuiltinArray::createArrayArrayFunc;
        }
        arrayLiteralExpr->code = llvmIRBuilder.CreateCall(createFunc, errorAlloca);
    } else {
        // TODO: 优化策略: 当size < 7时, 使用memset+store替代memcpy
        size_t size = arrayLiteralExpr->elements.size();
        LLVMValue *sizeLoad = llvmIRBuilder.getInt64(size);
        LLVMFunction *memcpyFunc = llvm::Intrinsic::getDeclaration(
                llvmModule.get(),
                llvm::Intrinsic::memcpy,
                Vector<LLVMType *>{
                        llvmIRBuilder.getInt8PtrTy(),
                        llvmIRBuilder.getInt8PtrTy(),
                        llvmIRBuilder.getInt64Ty()
                }
        );
        reportOnCodeGenError(!memcpyFunc, "Not found llvm.memcpy intrinsic function");
        if (elementType->equals(AtomicType::BOOLEAN_TYPE)) {
            // 避免使用vector<bool>
            Vector<unsigned char> literalVector(size);
            for (size_t i = 0; i < size; ++i) {
                literalVector[i] = staticPtrCast<BooleanLiteralExprNode>(arrayLiteralExpr->elements[i])->literal;
            }
            llvm::Constant *literalConstants = llvm::ConstantDataArray::get(llvmContext, literalVector);
            auto *globalLiteralConstants = new LLVMGlobalVariable(
                    *llvmModule,
                    literalConstants->getType(),
                    true,
                    LLVMGlobalValue::PrivateLinkage,
                    literalConstants
            );
            LLVMValue *literalListAlloca = llvmIRBuilder.CreateAlloca(
                    llvm::ArrayType::get(llvmIRBuilder.getInt8Ty(), size)
            );
            LLVMValue *castedLiteralListLoad = llvmIRBuilder.CreateBitCast(literalListAlloca, llvmIRBuilder.getInt8PtrTy());
            LLVMValue *castedLiteralConstants = llvmIRBuilder.CreateBitCast(globalLiteralConstants, llvmIRBuilder.getInt8PtrTy());
            Vector<LLVMValue *> memcpyArgs{
                    castedLiteralListLoad,
                    castedLiteralConstants,
                    llvmIRBuilder.getInt64(sizeof(unsigned char) * size),
                    llvmIRBuilder.getFalse()
            };
            llvmIRBuilder.CreateCall(memcpyFunc, memcpyArgs);
            LLVMValue *literalListPtr = llvmIRBuilder.CreateInBoundsGEP(
                    literalListAlloca,
                    Vector<LLVMValue *>{
                            llvmIRBuilder.getInt64(0),
                            llvmIRBuilder.getInt64(0)
                    }
            );
            Vector<LLVMValue *> createArgs{literalListPtr, sizeLoad, errorAlloca};
            arrayLiteralExpr->code = llvmIRBuilder.CreateCall(BuiltinArray::createBooleanArrayWithLiteralFunc, createArgs);
        } else if (elementType->equals(AtomicType::INTEGER_TYPE)) {
            Vector<long> literalVector(size);
            for (size_t i = 0; i < size; ++i) {
                literalVector[i] = staticPtrCast<IntegerLiteralExprNode>(arrayLiteralExpr->elements[i])->literal;
            }
            llvm::Constant *literalConstants = llvm::ConstantDataArray::get(llvmContext, literalVector);
            auto *globalLiteralConstants = new LLVMGlobalVariable(
                    *llvmModule,
                    literalConstants->getType(),
                    true,
                    LLVMGlobalValue::PrivateLinkage,
                    literalConstants
            );
            LLVMValue *literalListAlloca = llvmIRBuilder.CreateAlloca(
                    llvm::ArrayType::get(llvmIRBuilder.getInt64Ty(), size)
            );
            LLVMValue *castedLiteralListLoad = llvmIRBuilder.CreateBitCast(literalListAlloca, llvmIRBuilder.getInt8PtrTy());
            LLVMValue *castedLiteralConstants = llvmIRBuilder.CreateBitCast(globalLiteralConstants, llvmIRBuilder.getInt8PtrTy());
            Vector<LLVMValue *> memcpyArgs{
                    castedLiteralListLoad,
                    castedLiteralConstants,
                    llvmIRBuilder.getInt64(sizeof(long) * size),
                    llvmIRBuilder.getFalse()
            };
            llvmIRBuilder.CreateCall(memcpyFunc, memcpyArgs);
            LLVMValue *literalListPtr = llvmIRBuilder.CreateInBoundsGEP(
                    literalListAlloca,
                    Vector<LLVMValue *>{
                            llvmIRBuilder.getInt64(0),
                            llvmIRBuilder.getInt64(0)
                    }
            );
            Vector<LLVMValue *> createArgs{literalListPtr, sizeLoad, errorAlloca};
            arrayLiteralExpr->code = llvmIRBuilder.CreateCall(BuiltinArray::createIntegerArrayWithLiteralFunc, createArgs);
        } else if (elementType->equals(AtomicType::STRING_TYPE)) {
            ASTVisitor::visit(arrayLiteralExpr);

            LLVMValue *literalListAlloca = llvmIRBuilder.CreateAlloca(
                    llvm::ArrayType::get(BuiltinString::type, size)
            );

            for (size_t i = 0; i < size; ++i) {
                LLVMValue *literalItemPtr = llvmIRBuilder.CreateInBoundsGEP(
                        literalListAlloca, Vector<LLVMValue *>{
                                llvmIRBuilder.getInt64(0),
                                llvmIRBuilder.getInt64(i)
                        }
                );
                llvmIRBuilder.CreateStore(arrayLiteralExpr->elements[i]->code, literalItemPtr);
            }

            LLVMValue *literalListPtr = llvmIRBuilder.CreateInBoundsGEP(
                    literalListAlloca,
                    Vector<LLVMValue *>{
                            llvmIRBuilder.getInt64(0),
                            llvmIRBuilder.getInt64(0)
                    }
            );

            Vector<LLVMValue *> createArgs{literalListPtr, sizeLoad, errorAlloca};
            arrayLiteralExpr->code = llvmIRBuilder.CreateCall(BuiltinArray::createStringArrayWithLiteralFunc, createArgs);
        } else if (elementType->isArray()) {
            ASTVisitor::visit(arrayLiteralExpr);

            LLVMValue *literalListAlloca = llvmIRBuilder.CreateAlloca(
                    llvm::ArrayType::get(BuiltinArray::type, size)
            );

            for (size_t i = 0; i < size; ++i) {
                LLVMValue *literalItemPtr = llvmIRBuilder.CreateInBoundsGEP(
                        literalListAlloca, Vector<LLVMValue *>{
                                llvmIRBuilder.getInt64(0),
                                llvmIRBuilder.getInt64(i)
                        }
                );
                llvmIRBuilder.CreateStore(arrayLiteralExpr->elements[i]->code, literalItemPtr);
            }

            LLVMValue *literalListPtr = llvmIRBuilder.CreateInBoundsGEP(
                    literalListAlloca,
                    Vector<LLVMValue *>{
                            llvmIRBuilder.getInt64(0),
                            llvmIRBuilder.getInt64(0)
                    }
            );

            Vector<LLVMValue *> createArgs{literalListPtr, sizeLoad, errorAlloca};
            arrayLiteralExpr->code = llvmIRBuilder.CreateCall(BuiltinArray::createArrayArrayWithLiteralFunc, createArgs);
        }
    }
    LLVMValue *errorLoad = llvmIRBuilder.CreateLoad(errorAlloca);
    llvmIRBuilder.CreateCall(BuiltinError::exitIfErrorFunc, errorLoad);
}

void IRGenerator::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
    // 按右值处理
    varExpr->code = llvmIRBuilder.CreateLoad(varExpr->refVarDecl->code);
}

void IRGenerator::visit(const SharedPtr<CallExprNode> &callExpr) {
    ASTVisitor::visit(callExpr);
    llvm::Function *calleeFunc = llvmModule->getFunction(callExpr->calleeName);
    reportOnCodeGenError(!calleeFunc, "Function '" + callExpr->calleeName + "' is not found");
    if (calleeFunc->arg_size() != callExpr->args.size()) {
        reportCodeGenError("The number of parameters of function declaration and arguments of call expression does not match");
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
            LLVMValue *value = subExpr->code;
            LLVMValue *newValue;
            LLVMConstantInt *one = llvmIRBuilder.getInt64(1);

            if (uopExpr->opCode == StaticScriptLexer::PlusPlus) {
                newValue = llvmIRBuilder.CreateNSWAdd(value, one);
            } else {
                newValue = llvmIRBuilder.CreateNSWSub(value, one);
            }
            if (const auto &subVarExpr = dynPtrCast<IdentifierExprNode>(subExpr)) {
                llvmIRBuilder.CreateStore(newValue, subVarExpr->refVarDecl->code);
            } else if (const auto &asExpr = dynPtrCast<ArraySubscriptExprNode>(subExpr)) {
                setArrayElement(asExpr, newValue);
            }
            uopExpr->code = uopExpr->isPostfix ? value : newValue;
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


    /// 平凡赋值: lhs是左值, rhs是右值
    /// 符合赋值: lhs是左值也是右值, rhs是右值; `lhs op= rhs` 相当于 `lhs = lhs op rhs`
    if (bopCode == StaticScriptLexer::Assign) {
        bopExpr->rhs->accept(shared_from_this());
    } else {
        ASTVisitor::visit(bopExpr);
    }

    const SharedPtr<Type> &type = bopExpr->lhs->inferType;
    LLVMValue *lhsCode = bopExpr->lhs->code;
    LLVMValue *rhsCode = bopExpr->rhs->code;
    LLVMValue *targetCode = nullptr;

    switch (bopCode) {
        case StaticScriptLexer::Plus:
        case StaticScriptLexer::PlusAssign: {
            if (type->equals(AtomicType::STRING_TYPE)) {
                Vector<LLVMValue *> argsV{lhsCode, rhsCode};
                targetCode = llvmIRBuilder.CreateCall(BuiltinString::concatFunc, argsV);
            } else if (type->equals(AtomicType::INTEGER_TYPE)) {
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
            if (type->equals(AtomicType::STRING_TYPE)) {
                Vector<LLVMValue *> argsV{lhsCode, rhsCode};
                LLVMValue *relationship = llvmIRBuilder.CreateCall(BuiltinString::equalsFunc, argsV);
                targetCode = llvmIRBuilder.CreateICmpEQ(relationship, llvmIRBuilder.getInt64(0));
            } else {
                targetCode = llvmIRBuilder.CreateICmpEQ(lhsCode, rhsCode);
            }
            break;
        }
        case StaticScriptLexer::NotEquals: {
            if (type->equals(AtomicType::STRING_TYPE)) {
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
        // 在语义阶段保证lhs类型为IdentifierExprNode或者ArraySubscriptExprNode
        // 按左值处理
        if (bopCode == StaticScriptLexer::Assign) {
            targetCode = rhsCode;
        }
        if (const auto &leftVarExpr = dynPtrCast<IdentifierExprNode>(bopExpr->lhs)) {
            llvmIRBuilder.CreateStore(targetCode, leftVarExpr->refVarDecl->code);
        } else if (const auto &leftAsExpr = dynPtrCast<ArraySubscriptExprNode>(bopExpr->lhs)) {
            setArrayElement(leftAsExpr, targetCode);
        }
    }
    bopExpr->code = targetCode;
}

void IRGenerator::visit(const SharedPtr<ArraySubscriptExprNode> &asExpr) {
    ASTVisitor::visit(asExpr);
    SharedPtr<Type> iterType = asExpr->baseExpr->inferType;
    LLVMValue *iterCode = asExpr->baseExpr->code;
    LLVMValue *errorAlloca = llvmIRBuilder.CreateAlloca(BuiltinError::type);
    llvmIRBuilder.CreateStore(llvm::ConstantPointerNull::get(BuiltinError::type), errorAlloca);
    for (const SharedPtr<ExprNode> &indexExpr: asExpr->indexExprs) {
        LLVMFunction *getFunc = nullptr;
        SharedPtr<Type> iterEleType = staticPtrCast<ArrayType>(iterType)->getElementType();
        if (iterEleType->equals(AtomicType::BOOLEAN_TYPE)) {
            getFunc = BuiltinArray::getBooleanFunc;
        } else if (iterEleType->equals(AtomicType::INTEGER_TYPE)) {
            getFunc = BuiltinArray::getIntegerFunc;
        } else if (iterEleType->equals(AtomicType::STRING_TYPE)) {
            getFunc = BuiltinArray::getStringFunc;
        } else if (iterEleType->isArray()) {
            getFunc = BuiltinArray::getArrayFunc;
        }
        iterType = iterEleType;
        iterCode = llvmIRBuilder.CreateCall(getFunc, Vector<LLVMValue *>{iterCode, indexExpr->code, errorAlloca});
        LLVMValue *errorLoad = llvmIRBuilder.CreateLoad(errorAlloca);
        llvmIRBuilder.CreateCall(BuiltinError::exitIfErrorFunc, errorLoad);
    }
    asExpr->code = iterCode;
}