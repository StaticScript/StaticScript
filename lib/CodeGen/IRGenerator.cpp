#include "CodeGen/IRGenerator.h"
#include "Entity/Scope.h"

IRGenerator::IRGenerator() : llvmIRBuilder(llvmContext) {}

void IRGenerator::resolve(const SharedPtr<ModuleNode> &module) {
    llvmModule = makeShared<LLVMModule>(module->filename, llvmContext);
    ASTVisitor::resolve(module);
}

// TODO: 最外层作用域是否需要塞到main函数里?
void IRGenerator::visit(const SharedPtr<ModuleNode> &module) {
    LLVMFunctionType *mainFuncType = LLVMFunctionType::get(llvmIRBuilder.getInt64Ty(), false);
    LLVMFunction *mainFunc = LLVMFunction::Create(mainFuncType, LLVMFunction::ExternalLinkage, "main", llvmModule.get());
    LLVMBasicBlock *entryBlock = LLVMBasicBlock::Create(llvmContext, "entry", mainFunc);
    llvmIRBuilder.SetInsertPoint(entryBlock);
    ASTVisitor::visit(module);
    llvmIRBuilder.CreateRet(LLVMConstantInt::get(llvmContext, LLVMAPInt(64, 0)));
    module->code = mainFunc;
}
// delete
void IRGenerator::visit(const SharedPtr<BuiltinTypeNode> &builtinType) {
    ASTVisitor::visit(builtinType);
}

void IRGenerator::visit(const SharedPtr<VarDeclNode> &varDecl) {
    ASTVisitor::visit(varDecl);
    LLVMType *type = getType(varDecl->type);
    // 全局变量
    if (varDecl->scope->isTopLevel()) {
        llvmModule->getOrInsertGlobal(varDecl->name, type);
        LLVMGlobalVariable *gVar = llvmModule->getNamedGlobal(varDecl->name);
        // 设置链接性
        gVar->setLinkage(LLVMGlobalValue::CommonLinkage);
        // 设置const
        if (varDecl->modifier == VarModifier::Const) {
            gVar->setConstant(true);
        }
        // 如果是初始值为字面量
        if (dynPtrCast<LiteralExprNode>(varDecl->initVal)) {
            if (auto *constant = LLVMDynCast<LLVMConstantInt>(varDecl->initVal->code)) {
                gVar->setInitializer(constant);
            } else {
                // TODO: 全局变量为字符串类型的字面量
            }
        } else {
            // TODO: 全局变量初始值为表达式
            // C++中专门创建了函数对此类型的全局变量进行初始化
        }
    } else {
        LLVMValue *alloca = llvmIRBuilder.CreateAlloca(type);
        llvmIRBuilder.CreateStore(varDecl->initVal->code, alloca);
        varDecl->code = alloca;
    }
}

// TODO
void IRGenerator::visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) {
    ASTVisitor::visit(paramVarDecl);
}

// TODO
void IRGenerator::visit(const SharedPtr<FunctionDeclNode> &funcDecl) {
    Vector<LLVMType *> argsType;
    for (const SharedPtr<ParmVarDeclNode> &param: funcDecl->params) {
        if (param->type == BuiltinTypeNode::INTEGER_TYPE) {
            argsType.push_back(LLVMType::getInt64Ty(llvmContext));
        } else if (param->type == BuiltinTypeNode::BOOLEAN_TYPE) {
            argsType.push_back(LLVMType::getInt1Ty(llvmContext));
        } else if (param->type == BuiltinTypeNode::STRING_TYPE) {
            argsType.push_back(LLVMType::getInt8Ty(llvmContext));
        }
    }
    LLVMType *returnType = LLVMType::getVoidTy(llvmContext);
    if (funcDecl->returnType == BuiltinTypeNode::INTEGER_TYPE) {
        returnType = LLVMType::getInt64Ty(llvmContext);
    } else if (funcDecl->returnType == BuiltinTypeNode::BOOLEAN_TYPE) {
        returnType = LLVMType::getInt1Ty(llvmContext);
    } else if (funcDecl->returnType == BuiltinTypeNode::STRING_TYPE) {
        returnType = LLVMType::getInt8Ty(llvmContext);
    }
    LLVMFunctionType *funcType = LLVMFunctionType::get(returnType, argsType, false);
    LLVMFunction *func = LLVMFunction::Create(funcType, LLVMFunction::ExternalLinkage, funcDecl->name, llvmModule.get());
    unsigned idx = 0;
    for (auto &arg: func->args()) {
        arg.setName(funcDecl->params[idx]->name);
        funcDecl->params[idx]->code = &arg;
        ++idx;
    }
    LLVMBasicBlock *entryBlock = LLVMBasicBlock::Create(llvmContext, "entry", func);
    llvmIRBuilder.SetInsertPoint(entryBlock);
    funcDecl->body->accept(shared_from_this());
    LLVMVerifyFunction(*func);
    funcDecl->code = func;
}

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

// TODO
void IRGenerator::visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) {
//    strLiteralExpr->code = llvmIRBuilder.CreateGlobalString(strLiteralExpr->literal);
}

void IRGenerator::visit(const SharedPtr<IdentifierExprNode> &varExpr) {
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
    switch (uopExpr->opCode) {
        case StaticScriptLexer::Minus: {
            uopExpr->code = llvmIRBuilder.CreateNeg(uopExpr->code);
            break;
        }
        default: {
            throw CodeGenException("不支持的一元操作符");
        }
    }
}

void IRGenerator::visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) {
    // Plus...NotEquals
    if (bopExpr->opCode >= 22 && bopExpr->opCode <= 31) {
        ASTVisitor::visit(bopExpr);
    }
    switch (bopExpr->opCode) {
        case StaticScriptLexer::Assign: {
            bopExpr->rhs->accept(shared_from_this());
            // 在语义阶段保证lhs类型为IdentifierExprNode
            SharedPtr<IdentifierExprNode> varExpr = staticPtrCast<IdentifierExprNode>(bopExpr->lhs);
            llvmIRBuilder.CreateStore(bopExpr->rhs->code, varExpr->refVarDecl->code);
            break;
        }
        case StaticScriptLexer::Plus: {
            bopExpr->code = llvmIRBuilder.CreateAdd(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::Minus: {
            bopExpr->code = llvmIRBuilder.CreateSub(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::Multiply: {
            bopExpr->code = llvmIRBuilder.CreateMul(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::Divide: {
            bopExpr->code = llvmIRBuilder.CreateSDiv(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::LessThan: {
            bopExpr->code = llvmIRBuilder.CreateICmpSLT(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::GreaterThan: {
            bopExpr->code = llvmIRBuilder.CreateICmpSGT(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::LessThanEquals: {
            bopExpr->code = llvmIRBuilder.CreateICmpSLE(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::GreaterThanEquals: {
            bopExpr->code = llvmIRBuilder.CreateICmpSGE(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::Equals: {
            bopExpr->code = llvmIRBuilder.CreateICmpEQ(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::NotEquals: {
            bopExpr->code = llvmIRBuilder.CreateICmpNE(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        default: {
            throw CodeGenException("不支持的二元操作符");
        }
    }
}

void IRGenerator::visit(const SharedPtr<ExprStmtNode> &exprStmt) {
    ASTVisitor::visit(exprStmt);
}

void IRGenerator::visit(const SharedPtr<CompoundStmtNode> &compStmt) {
    ASTVisitor::visit(compStmt);
}

void IRGenerator::visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) {
    ASTVisitor::visit(varDeclStmt);
}

void IRGenerator::visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) {
    ASTVisitor::visit(funcDeclStmt);
}

// TODO: if statement ir gen
void IRGenerator::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ifStmt->condition->accept(shared_from_this());
    // 创建并插入比较指令
    LLVMValue *condition = llvmIRBuilder.CreateICmpEQ(
            ifStmt->condition->code,
            LLVMConstantInt::get(llvmContext, LLVMAPInt(1, 1)),
            "condition"
    );

    // 获取函数位置
    LLVMFunction *func = llvmIRBuilder.GetInsertBlock()->getParent();
    // 创建then/else/merge三个基本块
    LLVMBasicBlock *thenBB = LLVMBasicBlock::Create(llvmContext, "then", func);
    LLVMBasicBlock *elseBB = LLVMBasicBlock::Create(llvmContext, "else");
    LLVMBasicBlock *mergeBB = LLVMBasicBlock::Create(llvmContext, "merge", func);
    // 创建并插入条件跳转指令
    llvmIRBuilder.CreateCondBr(condition, thenBB, ifStmt->elseBody ? elseBB : mergeBB);

    // 设置插入点为then基本块
    llvmIRBuilder.SetInsertPoint(thenBB);
    // 生成then基本块内部的指令
    ifStmt->thenBody->accept(shared_from_this());
    // 创建并插入无条件跳转指令(then->merge)
    llvmIRBuilder.CreateBr(mergeBB);
    // then的codeGen可能会改变当前块, 因此需要复位一下
    thenBB = llvmIRBuilder.GetInsertBlock();

    if (ifStmt->elseBody) {
        func->getBasicBlockList().push_back(elseBB);
        llvmIRBuilder.SetInsertPoint(elseBB);
        ifStmt->elseBody->accept(shared_from_this());
        llvmIRBuilder.CreateBr(mergeBB);
        // else的codeGen可能会改变当前块, 因此需要复位一下
        elseBB = llvmIRBuilder.GetInsertBlock();
    }
    func->getBasicBlockList().push_back(mergeBB);
    llvmIRBuilder.SetInsertPoint(mergeBB);
    llvm::PHINode *phi = llvmIRBuilder.CreatePHI(LLVMType::getInt64Ty(llvmContext), 2);
    phi->addIncoming(ifStmt->thenBody->code, thenBB);
    phi->addIncoming(ifStmt->elseBody->code, elseBB);
}

void IRGenerator::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    ASTVisitor::visit(whileStmt);
}

void IRGenerator::visit(const SharedPtr<ForStmtNode> &forStmt) {
    ASTVisitor::visit(forStmt);
}

void IRGenerator::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {
    ASTVisitor::visit(continueStmt);
}

void IRGenerator::visit(const SharedPtr<BreakStmtNode> &breakStmt) {
    ASTVisitor::visit(breakStmt);
}

void IRGenerator::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
    if (returnStmt->returnExpr) {
        returnStmt->code = llvmIRBuilder.CreateRet(returnStmt->returnExpr->code);
    } else {
        returnStmt->code = llvmIRBuilder.CreateRetVoid();
    }
}
