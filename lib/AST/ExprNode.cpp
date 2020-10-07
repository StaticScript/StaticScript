#include "AST/ExprNode.h"
#include "AST/DeclNode.h"
#include <utility>

LiteralExprNode::LiteralExprNode(TypeKind type) : type(type) {}

IntegerLiteralExprNode::IntegerLiteralExprNode(TypeKind type, int literal) : LiteralExprNode(type), literal(literal) {}

BooleanLiteralExprNode::BooleanLiteralExprNode(TypeKind type, bool literal) : LiteralExprNode(type), literal(literal) {}

StringLiteralExprNode::StringLiteralExprNode(TypeKind type, String literal) : LiteralExprNode(type), literal(std::move(literal)) {}

IdentifierExprNode::IdentifierExprNode(String name) : name(std::move(name)) {}

UnaryOperatorExprNode::UnaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &subExpr) : opCode(opCode), subExpr(subExpr) {}

BinaryOperatorExprNode::BinaryOperatorExprNode(unsigned int opCode, const SharedPtr<ExprNode> &lhs, const SharedPtr<ExprNode> &rhs) : opCode(opCode),
                                                                                                                                      lhs(lhs), rhs(rhs) {}

CallExprNode::CallExprNode(String calleeName, const SharedPtrVector<ExprNode> &args) : calleeName(std::move(calleeName)), args(args) {}
