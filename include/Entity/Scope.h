#pragma once

#include <memory>
#include "AST/DeclNode.h"
#include "Support/Alias.h"
#include "Support/Find.h"

class TopLevelScope;

class LocalScope;

/// 作用域
class Scope : public std::enable_shared_from_this<Scope> {
public:
    Scope() = default;

    virtual ~Scope() = default;

    virtual void addChild(const SharedPtr<LocalScope> &scope) final;

    virtual void addVariable(const String &varName, const SharedPtr<VarDeclNode> &varDecl) final;

    virtual bool hasVariable(const String &name) final;

    virtual bool isTopLevel() = 0;

    virtual SharedPtr<TopLevelScope> getTopLevel() = 0;

    virtual SharedPtr<Scope> getParent() = 0;

    virtual SharedPtr<VarDeclNode> resolveVariable(const String &name) = 0;

    SharedPtr<Node> host;

protected:
    SharedPtrVector<LocalScope> children;
    SharedPtrMap<String, VarDeclNode> variables;
};

/// 顶级作用域
class TopLevelScope : public Scope {
public:
    static SharedPtr<TopLevelScope> create();

    TopLevelScope() = default;

    ~TopLevelScope() override = default;

    bool isTopLevel() override;

    SharedPtr<TopLevelScope> getTopLevel() override;

    SharedPtr<Scope> getParent() override;

    virtual void addFunction(const String &name, const SharedPtr<FunctionDeclNode> &funcDecl) final;

    virtual bool hasFunction(const String &name) final;

    SharedPtr<VarDeclNode> resolveVariable(const String &name) override;

    SharedPtr<FunctionDeclNode> resolveFunction(const String &name);

private:
    SharedPtrMap<String, FunctionDeclNode> functions;

    SharedPtrMap<String, FunctionDeclNode> libFunctions = FunctionDeclNode::getBuiltinFunctions();
};

/// 局部作用域
class LocalScope : public Scope {
public:
    static SharedPtr<LocalScope> create(const SharedPtr<Scope> &parent);

    explicit LocalScope(const SharedPtr<Scope> &parent);

    ~LocalScope() override = default;

    bool isTopLevel() override;

    SharedPtr<TopLevelScope> getTopLevel() override;

    SharedPtr<Scope> getParent() override;

    SharedPtr<VarDeclNode> resolveVariable(const String &name) override;

private:
    SharedPtr<Scope> parent;
};