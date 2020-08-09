#include "AST/Expr.h"
#include "AST/Decl.h"
#include <utility>

LiteralExpr::LiteralExpr(TypeKind type) : type(type) {}

IntegerLiteralExpr::IntegerLiteralExpr(TypeKind type, int literal) : LiteralExpr(type), literal(literal) {}

BooleanLiteralExpr::BooleanLiteralExpr(TypeKind type, bool literal) : LiteralExpr(type), literal(literal) {}

StringLiteralExpr::StringLiteralExpr(TypeKind type, String literal) : LiteralExpr(type), literal(std::move(literal)) {}

IdentifierExpr::IdentifierExpr(String name) : name(std::move(name)) {}

UnaryOperatorExpr::UnaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &subExpr) : operatorCode(operatorCode), subExpr(subExpr) {}

BinaryOperatorExpr::BinaryOperatorExpr(size_t operatorCode, const SharedPtr<Expr> &lhs, const SharedPtr<Expr> &rhs) : operatorCode(operatorCode),
                                                                                                                      lhs(lhs), rhs(rhs) {}

CallExpr::CallExpr(String calleeName, const SharedPtrVector<Expr> &args) : calleeName(std::move(calleeName)), args(args) {}
