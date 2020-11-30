#include "CodeGen/IRGenerator.h"

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