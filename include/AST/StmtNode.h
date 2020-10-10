#pragma once

#include "AST/Node.h"
#include "Util/Alias.h"

class VarDeclNode;

class FunctionDeclNode;

class ExprNode;

class StmtNode: public Node {
public:
    ~StmtNode() override = default;
};

// 值语句
class ExprStmtNode : public StmtNode {
public:
    SharedPtr<ExprNode> expr;

    explicit ExprStmtNode(const SharedPtr<ExprNode> &expr);

    ~ExprStmtNode() override = default;
};

// 复合语句
class CompoundStmtNode final : public StmtNode {
public:
    explicit CompoundStmtNode(const SharedPtrVector<StmtNode> &childStmts);

    ~CompoundStmtNode() override = default;

    [[nodiscard]] virtual bool isEmpty() const;

    SharedPtrVector<StmtNode> childStmts;
};

// 变量声明语句
class VarDeclStmtNode final : public StmtNode {
public:
    ~VarDeclStmtNode() override = default;

    virtual void pushVarDecl(const SharedPtr<VarDeclNode> &varDecl);

    SharedPtrVector<VarDeclNode> childVarDecls;
};

// 函数声明语句
class FunctionDeclStmtNode final : public StmtNode {
public:
    explicit FunctionDeclStmtNode(const SharedPtr<FunctionDeclNode> &childFunctionDecl);

    ~FunctionDeclStmtNode() override = default;

    SharedPtr<FunctionDeclNode> childFunctionDecl;
};

// if语句
class IfStmtNode : public StmtNode {
public:
    IfStmtNode(const SharedPtr<ExprNode> &condition, const SharedPtr<StmtNode> &thenStmt, const SharedPtr<StmtNode> &elseStmt);

    ~IfStmtNode() override = default;

    SharedPtr<ExprNode> condition;
    SharedPtr<StmtNode> thenBody;
    SharedPtr<StmtNode> elseBody;
};

// while语句
class WhileStmtNode : public StmtNode {
public:
    WhileStmtNode(const SharedPtr<ExprNode> &condition, const SharedPtr<StmtNode> &body);

    ~WhileStmtNode() override = default;

    SharedPtr<ExprNode> condition;
    SharedPtr<StmtNode> body;
};

// for语句
class ForStmtNode : public StmtNode {
public:
    ForStmtNode(const SharedPtr<VarDeclStmtNode> &forInitVarDecls, const SharedPtrVector<ExprNode> &forInitExprList, const SharedPtr<ExprNode> &forCondition,
                const SharedPtrVector<ExprNode> &forUpdate, const SharedPtr<StmtNode> &body);

    ~ForStmtNode() override = default;

    SharedPtr<VarDeclStmtNode> forInitVarDecls;
    SharedPtrVector<ExprNode> forInitExprList;
    SharedPtr<ExprNode> forCondition;
    SharedPtrVector<ExprNode> forUpdate;
    SharedPtr<StmtNode> body;
};

// continue语句
class ContinueStmtNode : public StmtNode {
public:
    ~ContinueStmtNode() override = default;
};

// break语句
class BreakStmtNode : public StmtNode {
public:
    ~BreakStmtNode() override = default;
};

// return语句
class ReturnStmtNode : public StmtNode {
public:
    explicit ReturnStmtNode(const SharedPtr<ExprNode> &argument);

    ~ReturnStmtNode() override = default;

    SharedPtr<ExprNode> argument;
};