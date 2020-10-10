#pragma once

#include "AST/Node.h"
#include "AST/TypeNode.h"
#include "AST/StmtNode.h"

class FunctionDeclNode;

class ExprNode: public Node {
public:
    ~ExprNode() override = default;
};

// 字面量表达式
class LiteralExprNode : public ExprNode {
public:
    explicit LiteralExprNode(TypeKind type);

    ~LiteralExprNode() override = default;

    TypeKind type;
};

// 布尔值字面量表达式
class BooleanLiteralExprNode : public LiteralExprNode {
public:
    explicit BooleanLiteralExprNode(TypeKind type, bool literal);

    ~BooleanLiteralExprNode() override = default;

    bool literal;
};

// 整数字面量表达式
class IntegerLiteralExprNode : public LiteralExprNode {
public:
    explicit IntegerLiteralExprNode(TypeKind type, int literal);

    ~IntegerLiteralExprNode() override = default;

    int literal;
};

// 字符串字面量表达式
class StringLiteralExprNode : public LiteralExprNode {
public:
    explicit StringLiteralExprNode(TypeKind type, String literal);

    ~StringLiteralExprNode() override = default;

    String literal;
};

// 括号表达式
class ParenExprNode : public ExprNode {
public:
    ~ParenExprNode() override = default;
};

// 标识符表达式
class IdentifierExprNode : public ExprNode {
public:
    explicit IdentifierExprNode(String name);

    ~IdentifierExprNode() override = default;

    String name;
};

// 函数调用表达式
class CallExprNode : public ExprNode {
public:

    CallExprNode(String calleeName, const SharedPtrVector<ExprNode> &args);

    ~CallExprNode() override = default;

    String calleeName;
    SharedPtr<FunctionDeclNode> callee;
    SharedPtrVector<ExprNode> args;
};

// 一元运算表达式
class UnaryOperatorExprNode : public ExprNode {
public:
    UnaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &subExpr);

    ~UnaryOperatorExprNode() override = default;

    unsigned int opCode;
    SharedPtr<ExprNode> subExpr;
};

// 二元运算表达式
class BinaryOperatorExprNode : public ExprNode {
public:
    BinaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &lhs, const SharedPtr<ExprNode> &rhs);

    ~BinaryOperatorExprNode() override = default;

    unsigned int opCode;
    SharedPtr<ExprNode> lhs, rhs;
};
