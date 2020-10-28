#include "CodeGen/IRGenerator.h"
#include "Entity/Scope.h"

IRGenerator::IRGenerator() : llvmIRBuilder(llvmContext) {}

void IRGenerator::resolve(const SharedPtr<ModuleNode> &module) {
    llvmModule = makeShared<LLVMModule>(module->filename, llvmContext);
    ASTVisitor::resolve(module);
    LLVMVerifyModule(*llvmModule);
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

// delete
void IRGenerator::visit(const SharedPtr<BuiltinTypeNode> &builtinType) {
    ASTVisitor::visit(builtinType);
}

void IRGenerator::visit(const SharedPtr<VarDeclNode> &varDecl) {
    ASTVisitor::visit(varDecl);
    LLVMType *type = getType(varDecl->type);
    // 全局变量
    if (varDecl->scope->isTopLevel()) {
        llvm::Constant *initConstant = nullptr;
        if (dynPtrCast<LiteralExprNode>(varDecl->initVal)) {
            // TODO: 此处没有考虑initVal为字符串的情况
            initConstant = LLVMDynCast<LLVMConstantInt>(varDecl->initVal->code);
        }
        auto *gVar = new LLVMGlobalVariable(
                *llvmModule,
                type,
                varDecl->isConstant(),
                LLVMGlobalValue::ExternalLinkage,
                initConstant,
                varDecl->name
        );
        if (!initConstant) {
            llvmIRBuilder.CreateStore(varDecl->initVal->code, gVar);
        }
        varDecl->code = gVar;
    } else {
        LLVMValue *alloca = llvmIRBuilder.CreateAlloca(type);
        if (varDecl->initVal) {
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

    {
        size_t i = 0;
        for (llvm::Argument &arg : func->args()) {
            arg.setName(funcDecl->params[i]->name);
            LLVMValue *paramAlloca = llvmIRBuilder.CreateAlloca(getType(funcDecl->params[i]->type));
            llvmIRBuilder.CreateStore(&arg, paramAlloca);
            funcDecl->params[i]->code = paramAlloca;
            i += 1;
        }
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

// TODO: 字符串字面量
void IRGenerator::visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) {
    strLiteralExpr->code = llvmIRBuilder.CreateGlobalString(strLiteralExpr->literal);
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
    switch (uopExpr->opCode) {
        case StaticScriptLexer::Minus: {
            uopExpr->code = llvmIRBuilder.CreateNSWNeg(uopExpr->subExpr->code);
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
            // 按左值处理
            SharedPtr<IdentifierExprNode> varExpr = staticPtrCast<IdentifierExprNode>(bopExpr->lhs);
            llvmIRBuilder.CreateStore(bopExpr->rhs->code, varExpr->refVarDecl->code);
            break;
        }
        case StaticScriptLexer::Plus: {
            bopExpr->code = llvmIRBuilder.CreateNSWAdd(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::Minus: {
            bopExpr->code = llvmIRBuilder.CreateNSWSub(bopExpr->lhs->code, bopExpr->rhs->code);
            break;
        }
        case StaticScriptLexer::Multiply: {
            bopExpr->code = llvmIRBuilder.CreateNSWMul(bopExpr->lhs->code, bopExpr->rhs->code);
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
    // 复位插入点
    setFuncInsertPoint(mainFn);
}

void IRGenerator::visit(const SharedPtr<IfStmtNode> &ifStmt) {
    ifStmt->condition->accept(shared_from_this());
    ifStmt->thenBB = createBasicBlock("if.then");
    ifStmt->endBB = createBasicBlock("if.end");
    ifStmt->elseBB = ifStmt->endBB;
    if (ifStmt->elseBody) {
        ifStmt->elseBB = createBasicBlock("if.else");
    }

    llvmIRBuilder.CreateCondBr(ifStmt->condition->code, ifStmt->thenBB, ifStmt->elseBB);

    emitBlock(ifStmt->thenBB);
    ifStmt->thenBody->accept(shared_from_this());
    emitBranch(ifStmt->endBB);

    if (ifStmt->elseBody) {
        emitBlock(ifStmt->elseBB);
        ifStmt->elseBody->accept(shared_from_this());
        emitBranch(ifStmt->endBB);
    }
    emitBlock(ifStmt->endBB, true);
}

// TODO: break和continue入栈出栈
void IRGenerator::visit(const SharedPtr<WhileStmtNode> &whileStmt) {
    whileStmt->condBB = createBasicBlock("while.cond");
    whileStmt->bodyBB = createBasicBlock("while.body");
    whileStmt->endBB = createBasicBlock("while.end");
    emitBlock(whileStmt->condBB);
    whileStmt->condition->accept(shared_from_this());
    llvmIRBuilder.CreateCondBr(whileStmt->condition->code, whileStmt->bodyBB, whileStmt->endBB);

    emitBlock(whileStmt->bodyBB);
    whileStmt->body->accept(shared_from_this());
    emitBranch(whileStmt->condBB);

    emitBlock(whileStmt->endBB, true);
}

// TODO: break和continue入栈出栈
void IRGenerator::visit(const SharedPtr<ForStmtNode> &forStmt) {
    if (forStmt->initVarStmt) {
        forStmt->initVarStmt->accept(shared_from_this());
    } else {
        for (const SharedPtr<ExprNode> &initExpr: forStmt->initExprs) {
            initExpr->accept(shared_from_this());
        }
    }
    forStmt->condBB = createBasicBlock("for.cond");
    forStmt->endBB = createBasicBlock("for.end");
    emitBlock(forStmt->condBB);

    if (!forStmt->updates.empty()) {
        forStmt->updateBB = createBasicBlock("for.update");
    }

    if (forStmt->condition) {
        forStmt->bodyBB = createBasicBlock("for.body");
        forStmt->condition->accept(shared_from_this());
        llvmIRBuilder.CreateCondBr(forStmt->condition->code, forStmt->bodyBB, forStmt->endBB);
        emitBlock(forStmt->bodyBB);
    } else {
        // 如果没有condition, 则把condBB看做bodyBB
    }

    forStmt->body->accept(shared_from_this());

    if (!forStmt->updates.empty()) {
        emitBlock(forStmt->updateBB);
        for (const SharedPtr<ExprNode> &updateExpr: forStmt->updates) {
            updateExpr->accept(shared_from_this());
        }
    }

    emitBranch(forStmt->condBB);

    emitBlock(forStmt->endBB, true);
}

void IRGenerator::visit(const SharedPtr<ContinueStmtNode> &continueStmt) {
    SharedPtr<WhileStmtNode> whileStmt = dynPtrCast<WhileStmtNode>(continueStmt->refIterationStmt);
    if (whileStmt) {
        llvmIRBuilder.CreateBr(whileStmt->condBB);
    } else {
        // 语义分析阶段保证了refIterationStmt不是WhileStmtNode类型就是ForStmtNode类型
        SharedPtr<ForStmtNode> forStmt = dynPtrCast<ForStmtNode>(continueStmt->refIterationStmt);
        llvmIRBuilder.CreateBr(forStmt->updateBB ? forStmt->updateBB : forStmt->condBB);
    }
}

void IRGenerator::visit(const SharedPtr<BreakStmtNode> &breakStmt) {
    SharedPtr<WhileStmtNode> whileStmt = dynPtrCast<WhileStmtNode>(breakStmt->refIterationStmt);
    if (whileStmt) {
        llvmIRBuilder.CreateBr(whileStmt->endBB);
    } else {
        SharedPtr<ForStmtNode> forStmt = dynPtrCast<ForStmtNode>(breakStmt->refIterationStmt);
        llvmIRBuilder.CreateBr(forStmt->endBB);
    }
}

// TODO: 处理函数中间的return语句
void IRGenerator::visit(const SharedPtr<ReturnStmtNode> &returnStmt) {
    ASTVisitor::visit(returnStmt);
    if (returnStmt->returnExpr) {
        llvmIRBuilder.CreateRet(returnStmt->returnExpr->code);
    } else {
        llvmIRBuilder.CreateRetVoid();
    }
}
