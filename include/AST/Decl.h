#ifndef STATICSCRIPT_AST_DECL_H
#define STATICSCRIPT_AST_DECL_H

#include <vector>
#include <string>
#include "AST/Base.h"
#include "AST/Type.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"

enum class VarModifier {
    Let, Const
};

class Decl : public ASTNode {
public:
    ~Decl() override = default;
};

// 变量声明
class VarDecl : public Decl {
public:
    ~VarDecl() override = default;

    void codegen() override;

    VarModifier modifier;
    SharedPtr<BuiltinType> type;
    std::string name;
    SharedPtr<Expr> initVal;
};

// 函数参数声明
class ParmVarDecl : public VarDecl {
public:
    ~ParmVarDecl() override = default;
};

// 函数声明
class FunctionDecl : public Decl {
public:
    ~FunctionDecl() override = default;

    SharedPtrVector<ParmVarDecl> params;
    SharedPtr<Type> returnType;
    SharedPtr<CompoundStmt> body;
};

#endif //STATICSCRIPT_AST_DECL_H
