#pragma once

#include "StaticScriptLexer.h"
#include "Sema/ASTVisitor.h"
#include "Util/Alias.h"
#include "Exception/CodeGenException.h"

class IRGenerator : public ASTVisitor {
public:
    explicit IRGenerator();

    void resolve(const SharedPtr<ModuleNode> &module) override;

    void visit(const SharedPtr<ModuleNode> &module) override;

    void visit(const SharedPtr<BuiltinTypeNode> &builtinType) override;

    void visit(const SharedPtr<VarDeclNode> &varDecl) override;

    void visit(const SharedPtr<ParmVarDeclNode> &paramVarDecl) override;

    void visit(const SharedPtr<FunctionDeclNode> &funcDecl) override;

    void visit(const SharedPtr<BooleanLiteralExprNode> &boolLiteralExpr) override;

    void visit(const SharedPtr<IntegerLiteralExprNode> &intLiteralExpr) override;

    void visit(const SharedPtr<StringLiteralExprNode> &strLiteralExpr) override;

    void visit(const SharedPtr<IdentifierExprNode> &varExpr) override;

    void visit(const SharedPtr<CallExprNode> &callExpr) override;

    void visit(const SharedPtr<UnaryOperatorExprNode> &uopExpr) override;

    void visit(const SharedPtr<BinaryOperatorExprNode> &bopExpr) override;

    void visit(const SharedPtr<ExprStmtNode> &exprStmt) override;

    void visit(const SharedPtr<CompoundStmtNode> &compStmt) override;

    void visit(const SharedPtr<VarDeclStmtNode> &varDeclStmt) override;

    void visit(const SharedPtr<FunctionDeclStmtNode> &funcDeclStmt) override;

    void visit(const SharedPtr<IfStmtNode> &ifStmt) override;

    void visit(const SharedPtr<WhileStmtNode> &whileStmt) override;

    void visit(const SharedPtr<ForStmtNode> &forStmt) override;

    void visit(const SharedPtr<ContinueStmtNode> &continueStmt) override;

    void visit(const SharedPtr<BreakStmtNode> &breakStmt) override;

    void visit(const SharedPtr<ReturnStmtNode> &returnStmt) override;

    LLVMModule &getLLVMModule() const {
        return *llvmModule;
    }

private:
    LLVMType *getType(const SharedPtr<BuiltinTypeNode> &builtinType) {
        LLVMType *type = llvmIRBuilder.getVoidTy();
        if (builtinType == BuiltinTypeNode::BOOLEAN_TYPE) {
            type = llvmIRBuilder.getInt1Ty();
        } else if (builtinType == BuiltinTypeNode::INTEGER_TYPE) {
            type = llvmIRBuilder.getInt64Ty();
        } else {
            // TODO: 字符串类型
        }
        return type;
    }

    void setFuncInsertPoint(LLVMFunction *func) {
        LLVMBasicBlock *curBB = &(func->getBasicBlockList().back());
        llvmIRBuilder.SetInsertPoint(curBB);
    }

    inline LLVMBasicBlock *createBasicBlock(const String &name, LLVMFunction *parent = nullptr, LLVMBasicBlock *before = nullptr) {
        return LLVMBasicBlock::Create(llvmContext, name, parent, before);
    }

    void emitBlock(LLVMBasicBlock *bb, bool isFinished = false) {
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

    void emitBranch(LLVMBasicBlock *targetBB) {
        LLVMBasicBlock *curBB = llvmIRBuilder.GetInsertBlock();
        if (!curBB || curBB->getTerminator()) {

        } else {
            llvmIRBuilder.CreateBr(targetBB);
        }
        llvmIRBuilder.ClearInsertionPoint();
    }

    LLVMContext llvmContext;
    LLVMIRBuilder llvmIRBuilder;
    SharedPtr<LLVMModule> llvmModule;

    LLVMFunction *mainFn = nullptr;
    LLVMFunction *curFn = nullptr;
};


