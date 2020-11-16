#include "Entity/Scope.h"
#include "CodeGen/IRGenerator.h"

void IRGenerator::visit(const SharedPtr<VarDeclNode> &varDecl) {
    ASTVisitor::visit(varDecl);
    LLVMType *type = getType(varDecl->type);
    // 变量有初始值
    bool hasInitVal = bool(varDecl->initVal);
    // 区分全局变量和局部变量
    if (varDecl->scope->isTopLevel()) {
        // 变量初始值为字面量
        bool isLiteralInit = bool(dynPtrCast<LiteralExprNode>(varDecl->initVal));
        // 是否为字符串类型变量
        bool isStringVar = varDecl->type == BuiltinTypeNode::STRING_TYPE;
        llvm::Constant *initializer;
        if (isStringVar) {
            initializer = llvm::ConstantPointerNull::getNullValue(type);
        } else {
            if (isLiteralInit) {
                initializer = LLVMCast<LLVMConstantInt>(varDecl->initVal->code);
            } else {
                initializer = llvm::ConstantInt::get(type, 0);
            }
        }
        auto *gVar = new LLVMGlobalVariable(
                *llvmModule,
                type,
                false,
                LLVMGlobalValue::ExternalLinkage,
                initializer,
                varDecl->name
        );
        uint64_t alignment = 8;
        if (varDecl->type == BuiltinTypeNode::BOOLEAN_TYPE) {
            alignment = 1;
        }
        gVar->setAlignment(llvm::MaybeAlign(alignment));
        // 如果为字符串或者有非字面量的初始值
        if (isStringVar || (hasInitVal && !isLiteralInit)) {
            llvmIRBuilder.CreateStore(varDecl->initVal->code, gVar);
        }
        varDecl->code = gVar;
    } else {
        LLVMValue *alloca = llvmIRBuilder.CreateAlloca(type);
        if (hasInitVal) {
            llvmIRBuilder.CreateStore(varDecl->initVal->code, alloca);
        }
        varDecl->code = alloca;
    }
}

// delete
void IRGenerator::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    ASTVisitor::visit(paramVarDecl);
}

void IRGenerator::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    Vector<LLVMType *> argsType;
    for (const SharedPtr<ParmVarDeclNode> &param: funcDecl->params) {
        argsType.push_back(getType(param->type));
    }
    LLVMType *returnType = getType(funcDecl->returnType);
    LLVMFunctionType *funcType = LLVMFunctionType::get(returnType, argsType, false);
    LLVMFunction *func = LLVMFunction::Create(funcType, LLVMFunction::ExternalLinkage, funcDecl->name, llvmModule.get());
    curFn = func;

    LLVMBasicBlock *entryBlock = createBasicBlock("entry", func);
    llvmIRBuilder.SetInsertPoint(entryBlock);

    for (size_t i = 0; i < func->arg_size(); ++i) {
        auto arg = func->getArg(i);
        arg->setName(funcDecl->params[i]->name);
        LLVMValue *paramAlloca = llvmIRBuilder.CreateAlloca(getType(funcDecl->params[i]->type));
        llvmIRBuilder.CreateStore(arg, paramAlloca);
        funcDecl->params[i]->code = paramAlloca;
    }

    funcDecl->body->accept(shared_from_this());

    // 如果返回类型为void而且没有显式return的话则添加ret指令
    if (funcDecl->returnType == nullptr) {
        LLVMBasicBlock *curBB = llvmIRBuilder.GetInsertBlock();
        if (!curBB->getTerminator()) {
            llvmIRBuilder.CreateRetVoid();
        }
    }
    LLVMVerifyFunction(*func);
}