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
        if (inputType->equals(AtomicType::BOOLEAN_TYPE)) {
            type = llvmIRBuilder.getInt1Ty();
        } else if (inputType->equals(AtomicType::INTEGER_TYPE)) {
            type = llvmIRBuilder.getInt64Ty();
        } else if (inputType->equals(AtomicType::STRING_TYPE)) {
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

void IRGenerator::setArrayElement(const SharedPtr<ArraySubscriptExprNode> &asExpr, LLVMValue *valueCode) {
    size_t indexExprsSize = asExpr->indexExprs.size();
    SharedPtr<Type> iterType = asExpr->baseExpr->inferType;
    LLVMValue *iterBaseCode = asExpr->baseExpr->code;
    LLVMValue *errorAlloca = llvmIRBuilder.CreateAlloca(BuiltinError::type);
    llvmIRBuilder.CreateStore(llvm::ConstantPointerNull::get(BuiltinError::type), errorAlloca);
    for (size_t i = 0; i < indexExprsSize; ++i) {
        LLVMFunction *accessFunc = nullptr;
        bool accessByGet = i < indexExprsSize - 1;
        const SharedPtr<Type> &iterEleType = staticPtrCast<ArrayType>(iterType)->getElementType();
        if (iterEleType->equals(AtomicType::BOOLEAN_TYPE)) {
            accessFunc = accessByGet ? BuiltinArray::getBooleanFunc : BuiltinArray::setBooleanFunc;
        } else if (iterEleType->equals(AtomicType::INTEGER_TYPE)) {
            accessFunc = accessByGet ? BuiltinArray::getIntegerFunc : BuiltinArray::setIntegerFunc;
        } else if (iterEleType->equals(AtomicType::STRING_TYPE)) {
            accessFunc = accessByGet ? BuiltinArray::getStringFunc : BuiltinArray::setStringFunc;
        } else if (iterEleType->isArray()) {
            accessFunc = accessByGet ? BuiltinArray::getArrayFunc : BuiltinArray::setArrayFunc;
        }
        iterType = iterEleType;
        Vector<LLVMValue *> accessArgs{
                iterBaseCode,
                asExpr->indexExprs[i]->code,
                errorAlloca
        };
        if (!accessByGet) {
            accessArgs.insert(accessArgs.begin() + 2, valueCode);
        }
        iterBaseCode = llvmIRBuilder.CreateCall(accessFunc, accessArgs);
        LLVMValue *errorLoad = llvmIRBuilder.CreateLoad(errorAlloca);
        llvmIRBuilder.CreateCall(BuiltinError::exitIfErrorFunc, errorLoad);
    }
}
