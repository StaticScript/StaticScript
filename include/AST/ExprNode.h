#pragma once

#include "AST/Node.h"
#include "AST/TypeNode.h"
#include "AST/StmtNode.h"

class FunctionDeclNode;

/// 表达式节点
class ExprNode : public Node {
public:
    ExprNode() = default;

    explicit ExprNode(const SharedPtr<BuiltinTypeNode> &type);

    ~ExprNode() override = default;

    SharedPtr<BuiltinTypeNode> inferType = nullptr;

    // 当前表达式的ir
    LLVMValue *code = nullptr;
};

/// 字面量表达式节点
class LiteralExprNode : public ExprNode {
public:
    explicit LiteralExprNode(const SharedPtr<BuiltinTypeNode> &type);

    LiteralExprNode() = default;

    ~LiteralExprNode() override = default;
};

/// 布尔值字面量表达式节点
class BooleanLiteralExprNode : public LiteralExprNode {
public:
    explicit BooleanLiteralExprNode(bool literal);

    ~BooleanLiteralExprNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    bool literal;
};

/// 整数字面量表达式节点
class IntegerLiteralExprNode : public LiteralExprNode {
public:
    explicit IntegerLiteralExprNode(int literal);

    ~IntegerLiteralExprNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    int literal;
};

/// 字符串字面量表达式节点
class StringLiteralExprNode : public LiteralExprNode {
public:
    explicit StringLiteralExprNode(String literal);

    StringLiteralExprNode() = default;

    ~StringLiteralExprNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    String literal;
};

/// 标识符表达式节点
class IdentifierExprNode : public ExprNode {
public:
    explicit IdentifierExprNode(String name);

    ~IdentifierExprNode() override = default;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    String name;

    SharedPtr<VarDeclNode> refVarDecl = nullptr;
};

/// 函数调用表达式节点
class CallExprNode : public ExprNode {
public:

    CallExprNode(String calleeName, const SharedPtrVector<ExprNode> &args);

    ~CallExprNode() override = default;

    void bindChildrenInversely() override;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    String calleeName;
    SharedPtrVector<ExprNode> args;

    SharedPtr<FunctionDeclNode> refFuncDecl = nullptr;
};

/// 一元运算表达式节点
class UnaryOperatorExprNode : public ExprNode {
public:
    UnaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &subExpr);

    ~UnaryOperatorExprNode() override = default;

    void bindChildrenInversely() override;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    unsigned int opCode;
    SharedPtr<ExprNode> subExpr = nullptr;
};

/// 二元运算表达式节点
class BinaryOperatorExprNode : public ExprNode {
public:
    BinaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &lhs, const SharedPtr<ExprNode> &rhs);

    ~BinaryOperatorExprNode() override = default;

    void bindChildrenInversely() override;

    void accept(const SharedPtr<ASTVisitor> &visitor) override;

    unsigned int opCode;
    SharedPtr<ExprNode> lhs = nullptr, rhs = nullptr;
};
