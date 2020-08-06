#include "AST/Expr.h"

#include <utility>

LiteralExpr::LiteralExpr(TypeKind type) : type(type) {}

IntegerLiteralExpr::IntegerLiteralExpr(TypeKind type, int literal) : LiteralExpr(type), literal(literal) {}

void IntegerLiteralExpr::codegen() {}

BooleanLiteralExpr::BooleanLiteralExpr(TypeKind type, bool literal) : LiteralExpr(type), literal(literal) {}

void BooleanLiteralExpr::codegen() {}

StringLiteralExpr::StringLiteralExpr(TypeKind type, std::string literal) : LiteralExpr(type), literal(std::move(literal)) {}

void StringLiteralExpr::codegen() {}

IdentifierExpr::IdentifierExpr(std::string name) : name(std::move(name)) {}

void IdentifierExpr::codegen() {}

void UnaryOperatorExpr::codegen() {

}

UnaryOperatorExpr::UnaryOperatorExpr() {}

UnaryOperatorExpr::UnaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &subExpr) : operatorCode(operatorCode), subExpr(subExpr) {}

BinaryOperatorExpr::BinaryOperatorExpr() {}

BinaryOperatorExpr::BinaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &lhs, const SharedPtr<Expr> &rhs) : operatorCode(operatorCode),
                                                                                                                      lhs(lhs), rhs(rhs) {}

void BinaryOperatorExpr::codegen() {

}

void CallExpr::codegen() {

}
