#pragma once

#include "StaticScriptLexer.h"
#include "Sema/ASTVisitor.h"
#include "CodeGen/Builtin.h"
#include "Util/Alias.h"
#include "Exception/CodeGenException.h"

class IRGenerator : public ASTVisitor {
public:
    explicit IRGenerator();

    void resolve(const SharedPtr<ModuleNode> &module) override;

    void visit(const SharedPtr<ModuleNode> &module) override;

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
    LLVMType *getType(const SharedPtr<BuiltinTypeNode> &builtinType);

    inline void setFuncInsertPoint(LLVMFunction *func) {
        LLVMBasicBlock *curBB = &(func->getBasicBlockList().back());
        llvmIRBuilder.SetInsertPoint(curBB);
    }

    inline LLVMBasicBlock *createBasicBlock(const String &name, LLVMFunction *parent = nullptr, LLVMBasicBlock *before = nullptr) {
        return LLVMBasicBlock::Create(llvmContext, name, parent, before);
    }

    void emitBlock(LLVMBasicBlock *bb, bool isFinished = false);

    void emitBranch(LLVMBasicBlock *targetBB);

    LLVMContext llvmContext;
    LLVMIRBuilder llvmIRBuilder;
    SharedPtr<LLVMModule> llvmModule;

    LLVMFunction *mainFn = nullptr;
    LLVMFunction *curFn = nullptr;
};


