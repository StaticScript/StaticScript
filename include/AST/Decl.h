#ifndef STATICSCRIPT_AST_DECL_H
#define STATICSCRIPT_AST_DECL_H

#include "AST/Type.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "Util/Alias.h"

enum class VarModifier {
    Let, Const, Param
};

class Decl {
public:
    virtual ~Decl() = default;
};

// 变量声明
class VarDecl : public Decl {
public:
    VarDecl();

    VarDecl(VarModifier modifier, String name, const SharedPtr<BuiltinType> &type);

    VarDecl(VarModifier modifier, String name, const SharedPtr<BuiltinType> &type, const SharedPtr<Expr> &defaultVal);

    ~VarDecl() override = default;

    VarModifier modifier;
    String name;
    SharedPtr<BuiltinType> type;
    SharedPtr<Expr> defaultVal;
};

// 函数参数声明
class ParmVarDecl : public VarDecl {
public:
    ParmVarDecl(const String &name, const SharedPtr<BuiltinType> &type);

    ~ParmVarDecl() override = default;
};

// 函数声明
class FunctionDecl : public Decl {
public:
    FunctionDecl(String name, const SharedPtrVector<ParmVarDecl> &params, const SharedPtr<BuiltinType> &returnType,
                 const SharedPtr<CompoundStmt> &body);

    ~FunctionDecl() override = default;

    String name;
    SharedPtrVector<ParmVarDecl> params;
    SharedPtr<BuiltinType> returnType;
    SharedPtr<CompoundStmt> body;
};

#endif // STATICSCRIPT_AST_DECL_H
