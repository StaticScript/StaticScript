#ifndef STATICSCRIPT_AST_EXPR_H
#define STATICSCRIPT_AST_EXPR_H

#include "AST/Type.h"
#include "AST/Stmt.h"

class FunctionDecl;

class Expr : public ValueStmt {
public:
    ~Expr() override = default;
};

// 字面量表达式
class LiteralExpr : public Expr {
public:
    explicit LiteralExpr(TypeKind type);

    ~LiteralExpr() override = default;

    TypeKind type;
};

// 布尔值字面量表达式
class BooleanLiteralExpr : public LiteralExpr {
public:
    explicit BooleanLiteralExpr(TypeKind type, bool literal);

    ~BooleanLiteralExpr() override = default;

    bool literal;

};

// 整数字面量表达式
class IntegerLiteralExpr : public LiteralExpr {
public:
    explicit IntegerLiteralExpr(TypeKind type, int literal);

    ~IntegerLiteralExpr() override = default;

    int literal;
};

// 字符串字面量表达式
class StringLiteralExpr : public LiteralExpr {
public:
    explicit StringLiteralExpr(TypeKind type, String literal);

    ~StringLiteralExpr() override = default;

    String literal;
};

// 括号表达式
class ParenExpr : public Expr {
public:
    ~ParenExpr() override = default;
};

// 标识符表达式
class IdentifierExpr : public Expr {
public:
    explicit IdentifierExpr(String name);

    ~IdentifierExpr() override = default;

    String name;
};

// 函数调用表达式
class CallExpr : public Expr {
public:

    CallExpr(String calleeName, const SharedPtrVector<Expr> &args);

    ~CallExpr() override = default;

    String calleeName;
    SharedPtr<FunctionDecl> callee;
    SharedPtrVector<Expr> args;
};

// 一元运算表达式
class UnaryOperatorExpr : public Expr {
public:
    UnaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &subExpr);

    ~UnaryOperatorExpr() override = default;

    size_t operatorCode{};
    SharedPtr<Expr> subExpr;
};

// 二元运算表达式
class BinaryOperatorExpr : public Expr {
public:
    BinaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &lhs, const SharedPtr<Expr> &rhs);

    ~BinaryOperatorExpr() override = default;

    size_t operatorCode;
    SharedPtr<Expr> lhs, rhs;
};

#endif //STATICSCRIPT_AST_EXPR_H
