#include "AST/Decl.h"
#include <utility>

VarDecl::VarDecl() : modifier(VarModifier::Let) {}

VarDecl::VarDecl(VarModifier modifier, String name, const SharedPtr<BuiltinType> &type) : modifier(modifier), name(std::move(name)), type(type) {}

VarDecl::VarDecl(VarModifier modifier, String name, const SharedPtr<BuiltinType> &type, const SharedPtr<Expr> &defaultVal) : modifier(modifier),
                                                                                                                             name(std::move(name)),
                                                                                                                             type(type),
                                                                                                                             defaultVal(defaultVal) {}

ParmVarDecl::ParmVarDecl(const String &name, const SharedPtr<BuiltinType> &type) : VarDecl(VarModifier::Param, name, type) {}

FunctionDecl::FunctionDecl(String name, const SharedPtrVector<ParmVarDecl> &params, const SharedPtr<BuiltinType> &returnType,
                           const SharedPtr<CompoundStmt> &body) : name(std::move(name)), params(params), returnType(returnType), body(body) {}
