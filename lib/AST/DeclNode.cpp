#include <utility>
#include "AST/DeclNode.h"
#include "Sema/ASTVisitor.h"

VarDeclNode::VarDeclNode() : modifier(VarModifier::Let) {}

VarDeclNode::VarDeclNode(
        VarModifier modifier,
        String name,
        const SharedPtr<Type> &type
) : modifier(modifier), name(std::move(name)), type(type) {}

VarDeclNode::VarDeclNode(VarModifier modifier,
                         String name,
                         const SharedPtr<Type> &type,
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
    if (initVal) {
        initVal->parent = self;
    }
}

bool VarDeclNode::isConstant() const {
    return modifier == VarModifier::Const;
}

ParmVarDeclNode::ParmVarDeclNode(
        const String &name,
        const SharedPtr<Type> &type
) : VarDeclNode(VarModifier::Param, name, type) {}

void ParmVarDeclNode::accept(const SharedPtr<ASTVisitor> &visitor) {
    visitor->visit(staticPtrCast<ParmVarDeclNode>(shared_from_this()));
}

SharedPtrMap<String, FunctionDeclNode> FunctionDeclNode::getBuiltinFunctions() {
    SharedPtrMap<String, FunctionDeclNode> functions;
    SharedPtr<ParmVarDeclNode> integerArg = makeShared<ParmVarDeclNode>("number", AtomicType::INTEGER_TYPE);
    SharedPtr<ParmVarDeclNode> strArg = makeShared<ParmVarDeclNode>("str", AtomicType::STRING_TYPE);
    SharedPtrVector<ParmVarDeclNode> integerArgs{integerArg};
    SharedPtrVector<ParmVarDeclNode> strArgs{strArg};
    functions["ss_integer2string"] = makeShared<FunctionDeclNode>("ss_integer2string", integerArgs, AtomicType::STRING_TYPE, nullptr);
    functions["ss_string2integer"] = makeShared<FunctionDeclNode>("ss_string2integer", strArgs, AtomicType::INTEGER_TYPE, nullptr);
    functions["ss_print_integer"] = makeShared<FunctionDeclNode>("ss_print_integer", integerArgs, nullptr, nullptr);
    functions["ss_println_integer"] = makeShared<FunctionDeclNode>("ss_println_integer", integerArgs, nullptr, nullptr);
    functions["ss_print_string"] = makeShared<FunctionDeclNode>("ss_print_string", strArgs, nullptr, nullptr);
    functions["ss_println_string"] = makeShared<FunctionDeclNode>("ss_println_string", strArgs, nullptr, nullptr);
    return functions;
}

FunctionDeclNode::FunctionDeclNode(
        String name,
        const SharedPtrVector<ParmVarDeclNode> &params,
        const SharedPtr<Type> &returnType,
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
    body->parent = self;
}
