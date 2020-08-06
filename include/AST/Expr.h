#ifndef STATICSCRIPT_AST_EXPR_H
#define STATICSCRIPT_AST_EXPR_H

#include <string>
#include "AST/Type.h"
#include "AST/Stmt.h"

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

    void codegen() override;

    bool literal;

};

// 整数字面量表达式
class IntegerLiteralExpr : public LiteralExpr {
public:
    explicit IntegerLiteralExpr(TypeKind type, int literal);

    ~IntegerLiteralExpr() override = default;

    void codegen() override;

    int literal;
};

// 字符串字面量表达式
class StringLiteralExpr : public LiteralExpr {
public:
    explicit StringLiteralExpr(TypeKind type, std::string literal);

    ~StringLiteralExpr() override = default;

    void codegen() override;

    std::string literal;
};

// 括号表达式
class ParenExpr : public Expr {
public:
    ~ParenExpr() override = default;
};

// 标识符表达式
class IdentifierExpr : public Expr {
public:
    explicit IdentifierExpr(std::string name);

    ~IdentifierExpr() override = default;

    void codegen() override;

    std::string name;
};

// 函数调用表达式
class CallExpr : public Expr {
public:
    ~CallExpr() override = default;

    void codegen() override;
};

// 一元运算表达式
class UnaryOperatorExpr : public Expr {
public:
    UnaryOperatorExpr();

    UnaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &subExpr);

    ~UnaryOperatorExpr() override = default;

    void codegen() override;

    size_t operatorCode;
    SharedPtr<Expr> subExpr;
};

// 二元运算表达式
class BinaryOperatorExpr : public Expr {
public:
    BinaryOperatorExpr();

    BinaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &lhs, const SharedPtr<Expr> &rhs);

    ~BinaryOperatorExpr() override = default;

    void codegen() override;

    size_t operatorCode;
    SharedPtr<Expr> lhs, rhs;
};

#endif //STATICSCRIPT_AST_EXPR_H
