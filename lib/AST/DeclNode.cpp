#include "AST/DeclNode.h"
#include <utility>

VarDeclNode::VarDeclNode() : modifier(VarModifier::Let) {}

VarDeclNode::VarDeclNode(VarModifier modifier, String name, const SharedPtr<BuiltinTypeNode> &type) : modifier(modifier), name(std::move(name)), type(type) {}

VarDeclNode::VarDeclNode(VarModifier modifier, String name, const SharedPtr<BuiltinTypeNode> &type, const SharedPtr<ExprNode> &defaultVal) : modifier(modifier),
                                                                                                                                             name(std::move(name)),
                                                                                                                                             type(type),
                                                                                                                                             defaultVal(defaultVal) {}

ParmVarDeclNode::ParmVarDeclNode(const String &name, const SharedPtr<BuiltinTypeNode> &type) : VarDeclNode(VarModifier::Param, name, type) {}

FunctionDeclNode::FunctionDeclNode(String name, const SharedPtrVector<ParmVarDeclNode> &params, const SharedPtr<BuiltinTypeNode> &returnType,
                                   const SharedPtr<CompoundStmtNode> &body) : name(std::move(name)), params(params), returnType(returnType), body(body) {}
