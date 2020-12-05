#include "Entity/Scope.h"
#include "CodeGen/IRGenerator.h"

void IRGenerator::visit(const SharedPtr<VarDeclNode> &varDecl) {
    ASTVisitor::visit(varDecl);
    LLVMType *type = getType(varDecl->type);
    LLVMValue *varInitCode = varDecl->initVal->code;

    bool isLiteralInit = bool(dynPtrCast<LiteralExprNode>(varDecl->initVal));
    bool declBoolean = varDecl->type->isBoolean();
    bool declFloat = varDecl->type->isFloat();
    bool initFloat = varDecl->initVal->type->isFloat();
    bool declString = varDecl->type->isString();
    bool declArray = varDecl->type->isArray();

    if (declFloat && !initFloat) {
        varInitCode = integer2float(varInitCode);
    } else if (!declFloat && initFloat) {
        varInitCode = float2integer(varInitCode);
    }

    // 区分全局变量和局部变量
    if (varDecl->scope->isTopLevel()) {
        llvm::Constant *initializer;
        if (declString || declArray) {
            initializer = llvm::ConstantPointerNull::getNullValue(type);
        } else if (isLiteralInit) {
            if (declFloat) {
                initializer = llvm::cast<llvm::ConstantFP>(varInitCode);
            } else {
                initializer = llvm::cast<LLVMConstantInt>(varInitCode);
            }
        } else {
            if (declFloat) {
                // 初始值为浮点表达式
                initializer = llvm::ConstantFP::get(type, 0.0);
            } else {
                // 初始值为布尔和整数表达式
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
        if (declBoolean) {
            alignment = 1;
        }
        gVar->setAlignment(llvm::MaybeAlign(alignment));
        // 在main函数中store字符串和数组的指针值或者非字面量的表达式值
        // 全局变量初始值为字面量时不需要在store varInitCode
        if (declString || declArray || !isLiteralInit) {
            llvmIRBuilder.CreateStore(varInitCode, gVar);
        }
        varDecl->code = gVar;
    } else {
        LLVMValue *alloca = llvmIRBuilder.CreateAlloca(type);
        llvmIRBuilder.CreateStore(varInitCode, alloca);
        varDecl->code = alloca;
    }
}

// delete
void IRGenerator::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    ASTVisitor::visit(paramVarDecl);
}

void IRGenerator::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    Vector<LLVMType *> argsType;
    for (const auto &param: funcDecl->params) {
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
    llvm::verifyFunction(*func);
    curFn = mainFn;
}