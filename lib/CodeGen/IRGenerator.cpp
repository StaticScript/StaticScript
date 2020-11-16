#include "CodeGen/IRGenerator.h"
#include "CodeGen/Pass.h"

IRGenerator::IRGenerator() : llvmIRBuilder(llvmContext) {}

void IRGenerator::resolve(const SharedPtr<ModuleNode> &module) {
    llvmModule = makeShared<LLVMModule>(module->filename, llvmContext);
    Builtin::initialize(*llvmModule, llvmContext);
    ASTVisitor::resolve(module);
    LLVMVerifyModule(*llvmModule);
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
    LLVMVerifyFunction(*mainFn);
}

LLVMType *IRGenerator::getType(const SharedPtr<BuiltinTypeNode> &builtinType) {
    LLVMType *type = llvmIRBuilder.getVoidTy();
    if (builtinType == BuiltinTypeNode::BOOLEAN_TYPE) {
        type = llvmIRBuilder.getInt1Ty();
    } else if (builtinType == BuiltinTypeNode::INTEGER_TYPE) {
        type = llvmIRBuilder.getInt64Ty();
    } else if (builtinType == BuiltinTypeNode::STRING_TYPE) {
        type = BuiltinString::type;
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
