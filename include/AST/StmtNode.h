#pragma once

#include "AST/Node.h"

class VarDeclNode;

class FunctionDeclNode;

class ExprNode;

/// 语句节点
class StmtNode : public Node {
public:
    ~StmtNode() override = default;
};

/// 表达式语句节点
class ExprStmtNode final : public StmtNode {
public:
    explicit ExprStmtNode(const SharedPtr<ExprNode> &expr);

    ~ExprStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtr<ExprNode> expr;
};

/// 复合语句节点
class CompoundStmtNode final : public StmtNode {
public:
    explicit CompoundStmtNode(const SharedPtrVector<StmtNode> &childStmts);

    ~CompoundStmtNode() override = default;

    [[nodiscard]] virtual bool isEmpty() const;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtrVector<StmtNode> childStmts;

    SharedPtr<Scope> internalScope;
};

/// 变量声明语句节点
class VarDeclStmtNode final : public StmtNode {
public:

    ~VarDeclStmtNode() override = default;

    virtual void pushVarDecl(const SharedPtr<VarDeclNode> &varDecl);

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtrVector<VarDeclNode> childVarDecls;
};

/// 函数声明语句节点
class FunctionDeclStmtNode final : public StmtNode {
public:
    explicit FunctionDeclStmtNode(const SharedPtr<FunctionDeclNode> &childFunctionDecl);

    ~FunctionDeclStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtr<FunctionDeclNode> childFunctionDecl;
};

/// if语句节点
class IfStmtNode final : public StmtNode {
public:
    IfStmtNode(
            const SharedPtr<ExprNode> &condition,
            const SharedPtr<StmtNode> &thenStmt,
            const SharedPtr<StmtNode> &elseStmt
    );

    ~IfStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtr<ExprNode> condition;
    SharedPtr<StmtNode> thenBody;
    SharedPtr<StmtNode> elseBody;
};

/// while语句节点
class WhileStmtNode final : public StmtNode {
public:
    WhileStmtNode(const SharedPtr<ExprNode> &condition, const SharedPtr<StmtNode> &body);

    ~WhileStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtr<ExprNode> condition;
    SharedPtr<StmtNode> body;
};

/// for语句节点
class ForStmtNode final : public StmtNode {
public:
    ForStmtNode(
            const SharedPtr<VarDeclStmtNode> &forInitVarDecls,
            const SharedPtrVector<ExprNode> &forInitExprList,
            const SharedPtr<ExprNode> &forCondition,
            const SharedPtrVector<ExprNode> &forUpdate,
            const SharedPtr<StmtNode> &body
    );

    ~ForStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtr<VarDeclStmtNode> forInitVarDecls;
    SharedPtrVector<ExprNode> forInitExprList;
    SharedPtr<ExprNode> forCondition;
    SharedPtrVector<ExprNode> forUpdate;
    SharedPtr<StmtNode> body;

    SharedPtr<Scope> internalScope;
};

/// continue语句节点
class ContinueStmtNode final : public StmtNode {
public:
    ~ContinueStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;
};


/// break语句节点
class BreakStmtNode final : public StmtNode {
public:
    ~BreakStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;
};

/// return语句节点
class ReturnStmtNode final : public StmtNode {
public:
    explicit ReturnStmtNode(const SharedPtr<ExprNode> &argument);

    ~ReturnStmtNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    SharedPtr<ExprNode> returnExpr;
};