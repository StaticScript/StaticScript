#include "AST/Expr.h"
#include "AST/Decl.h"
#include <utility>

LiteralExpr::LiteralExpr(TypeKind type) : type(type) {}

IntegerLiteralExpr::IntegerLiteralExpr(TypeKind type, int literal) : LiteralExpr(type), literal(literal) {}

BooleanLiteralExpr::BooleanLiteralExpr(TypeKind type, bool literal) : LiteralExpr(type), literal(literal) {}

StringLiteralExpr::StringLiteralExpr(TypeKind type, String literal) : LiteralExpr(type), literal(std::move(literal)) {}

IdentifierExpr::IdentifierExpr(String name) : name(std::move(name)) {}

UnaryOperatorExpr::UnaryOperatorExpr(unsigned int opCode, const SharedPtr<Expr> &subExpr) : opCode(opCode), subExpr(subExpr) {}

BinaryOperatorExpr::BinaryOperatorExpr(unsigned int opCode, const SharedPtr<Expr> &lhs, const SharedPtr<Expr> &rhs) : opCode(opCode),
                                                                                                                      lhs(lhs), rhs(rhs) {}

CallExpr::CallExpr(String calleeName, const SharedPtrVector<Expr> &args) : calleeName(std::move(calleeName)), args(args) {}
