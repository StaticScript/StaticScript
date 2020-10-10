#pragma once

#include "AST/Node.h"
#include "AST/TypeNode.h"
#include "AST/StmtNode.h"
#include "AST/ExprNode.h"
#include "Util/Alias.h"

enum class VarModifier {
    Let, Const, Param
};

class DeclNode : public Node {
public:
    ~DeclNode() override = default;
};

// 变量声明
class VarDeclNode : public DeclNode {
public:
    VarDeclNode();

    VarDeclNode(VarModifier modifier, String name, const SharedPtr<BuiltinTypeNode> &type);

    VarDeclNode(VarModifier modifier, String name, const SharedPtr<BuiltinTypeNode> &type, const SharedPtr<ExprNode> &defaultVal);

    ~VarDeclNode() override = default;

    VarModifier modifier;
    String name;
    SharedPtr<BuiltinTypeNode> type;
    SharedPtr<ExprNode> defaultVal;
};

// 函数参数声明
class ParmVarDeclNode : public VarDeclNode {
public:
    ParmVarDeclNode(const String &name, const SharedPtr<BuiltinTypeNode> &type);

    ~ParmVarDeclNode() override = default;
};

// 函数声明
class FunctionDeclNode : public DeclNode {
public:
    FunctionDeclNode(String name, const SharedPtrVector<ParmVarDeclNode> &params, const SharedPtr<BuiltinTypeNode> &returnType,
                     const SharedPtr<CompoundStmtNode> &body);

    ~FunctionDeclNode() override = default;

    String name;
    SharedPtrVector<ParmVarDeclNode> params;
    SharedPtr<BuiltinTypeNode> returnType;
    SharedPtr<CompoundStmtNode> body;
};
