#include "CodeGen/IRGenerator.h"
#include "CodeGen/Pass.h"

IRGenerator::IRGenerator() : llvmIRBuilder(llvmContext) {}

void IRGenerator::resolve(const SharedPtr<ModuleNode> &module, const llvm::Twine &libDir) {
    llvmModule = makeShared<LLVMModule>(module->filename, llvmContext);
    Builtin::initialize(*llvmModule, llvmContext, libDir);
    ASTVisitor::resolve(module);
    llvm::verifyModule(*llvmModule);
    runPasses(*llvmModule);
}

void IRGenerator::visit(const SharedPtr<ModuleNode> &module) {
    LLVMFunctionType *mainFnType = LLVMFunctionType::get(llvmIRBuilder.getInt64Ty(), false);
    mainFn = LLVMFunction::Create(mainFnType, LLVMFunction::ExternalLinkage, "main", llvmModule.get());
    curFn = mainFn;
    LLVMBasicBlock *mainEntryBlock = LLVMBasicBlock::Create(llvmContext, "entry", mainFn);
    llvmIRBuilder.SetInsertPoint(mainEntryBlock);
    ASTVisitor::visit(module);
    setFuncInsertPoint(mainFn);
    llvmIRBuilder.CreateRet(llvmIRBuilder.getInt64(0));
    llvm::verifyFunction(*mainFn);
}

LLVMType *IRGenerator::getType(const SharedPtr<Type> &inputType) {
    LLVMType *type = llvmIRBuilder.getVoidTy();
    if (inputType) {
        if (inputType->isBoolean()) {
            type = llvmIRBuilder.getInt1Ty();
        } else if (inputType->isInteger()) {
            type = llvmIRBuilder.getInt64Ty();
        } else if (inputType->isFloat()) {
            type = llvmIRBuilder.getDoubleTy();
        } else if (inputType->isString()) {
            type = BuiltinString::type;
        } else if (inputType->isArray()) {
            type = BuiltinArray::type;
        }
    }
    return type;
}

void IRGenerator::emitBlock(LLVMBasicBlock *bb, bool isFinished) {
    LLVMBasicBlock *curBB = llvmIRBuilder.GetInsertBlock();
    emitBranch(bb);
    if (isFinished && bb->use_empty()) {
        delete bb;
        return;
    }
    if (curBB && curBB->getParent()) {
        curFn->getBasicBlockList().insertAfter(curBB->getIterator(), bb);
    } else {
        curFn->getBasicBlockList().push_back(bb);
    }
    llvmIRBuilder.SetInsertPoint(bb);
}

void IRGenerator::emitBranch(LLVMBasicBlock *targetBB) {
    LLVMBasicBlock *curBB = llvmIRBuilder.GetInsertBlock();
    if (!curBB || curBB->getTerminator()) {

    } else {
        llvmIRBuilder.CreateBr(targetBB);
    }
    llvmIRBuilder.ClearInsertionPoint();
}

// 设置数组元素的值, 务必确保baseExpr已经被visit过
void IRGenerator::setArrayElement(const SharedPtr<ArraySubscriptExprNode> &asExpr, LLVMValue *valueCode) {
    size_t indexExprsSize = asExpr->indexExprs.size();
    SharedPtr<Type> iterType = asExpr->baseExpr->type;
    LLVMValue *iterBaseCode = asExpr->baseExpr->code;
    LLVMValue *errorAlloca = llvmIRBuilder.CreateAlloca(BuiltinError::type);
    llvmIRBuilder.CreateStore(llvm::ConstantPointerNull::get(BuiltinError::type), errorAlloca);
    for (size_t i = 0; i < indexExprsSize; ++i) {
        LLVMFunction *accessFunc = nullptr;
        bool accessBySet = i == indexExprsSize - 1;
        const SharedPtr<Type> &iterEleType = iterType->asArray()->getElementType();
        if (iterEleType->isBoolean()) {
            accessFunc = accessBySet ? BuiltinArray::setBooleanFunc : BuiltinArray::getBooleanFunc;
        } else if (iterEleType->isInteger()) {
            accessFunc = accessBySet ? BuiltinArray::setIntegerFunc : BuiltinArray::getIntegerFunc;
        } else if (iterEleType->isFloat()) {
            accessFunc = accessBySet ? BuiltinArray::setFloatFunc : BuiltinArray::getFloatFunc;
        } else if (iterEleType->isString()) {
            accessFunc = accessBySet ? BuiltinArray::setStringFunc : BuiltinArray::getStringFunc;
        } else if (iterEleType->isArray()) {
            accessFunc = accessBySet ? BuiltinArray::setArrayFunc : BuiltinArray::getArrayFunc;
        }
        iterType = iterEleType;
        Vector<LLVMValue *> accessArgs{
                iterBaseCode,
                asExpr->indexExprs[i]->code,
                errorAlloca
        };
        // 准备给多维数组最内层的元素赋值
        if (accessBySet) {
            // getFunc和setFunc参数不同, setFunc的第三个参数是需要赋的值
            accessArgs.insert(accessArgs.begin() + 2, valueCode);
        }
        iterBaseCode = llvmIRBuilder.CreateCall(accessFunc, accessArgs);
        LLVMValue *errorLoad = llvmIRBuilder.CreateLoad(errorAlloca);
        llvmIRBuilder.CreateCall(BuiltinError::exitIfErrorFunc, errorLoad);
    }
}
