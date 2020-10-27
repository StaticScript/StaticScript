#include <utility>
#include "AST/DeclNode.h"
#include "Sema/ASTVisitor.h"

VarDeclNode::VarDeclNode() : modifier(VarModifier::Let) {}

VarDeclNode::VarDeclNode(
        VarModifier modifier,
        String name,
        const SharedPtr<BuiltinTypeNode> &type
) : modifier(modifier), name(std::move(name)), type(type) {}

VarDeclNode::VarDeclNode(VarModifier modifier,
                         String name,
                         const SharedPtr<BuiltinTypeNode> &type,
                         const SharedPtr<ExprNode> &initVal
) : modifier(modifier),
    name(std::move(name)),
    type(type),
    initVal(initVal) {}

void VarDeclNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<VarDeclNode>(shared_from_this()));
}

void VarDeclNode::bindChildrenInversely() {
    auto self = shared_from_this();
    if (type) {
        type->parent = self;
    }
    if (initVal) {
        initVal->parent = self;
    }
}

bool VarDeclNode::isConstant() const {
    return modifier == VarModifier::Const;
}

ParmVarDeclNode::ParmVarDeclNode(
        const String &name,
        const SharedPtr<BuiltinTypeNode> &type
) : VarDeclNode(VarModifier::Param, name, type) {}

void ParmVarDeclNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ParmVarDeclNode>(shared_from_this()));
}

FunctionDeclNode::FunctionDeclNode(
        String name,
        const SharedPtrVector<ParmVarDeclNode> &params,
        const SharedPtr<BuiltinTypeNode> &returnType,
        const SharedPtr<CompoundStmtNode> &body
) : name(std::move(name)),
    params(params),
    returnType(returnType),
    body(body) {}

void FunctionDeclNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<FunctionDeclNode>(shared_from_this()));
}

void FunctionDeclNode::bindChildrenInversely() {
    auto self = shared_from_this();
    for (const SharedPtr<ParmVarDeclNode> &param: params) {
        param->parent = self;
    }
    if (returnType) {
        returnType->parent = self;
    }
    body->parent = self;
}
