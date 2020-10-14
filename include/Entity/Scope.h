#pragma once

#include <memory>
#include "AST/AST.h"
#include "Util/Alias.h"
#include "Util/Find.h"

class TopLevelScope;

class LocalScope;

class Scope : public std::enable_shared_from_this<Scope> {
public:
    Scope() = default;

    virtual ~Scope() = default;

    virtual void addChild(const SharedPtr<LocalScope> &scope);

    virtual void addVariable(const String &varName, const SharedPtr<VarDeclNode> &varDecl) final;

    virtual bool isTopLevel() = 0;

    virtual SharedPtr<TopLevelScope> getTopLevel() = 0;

    virtual SharedPtr<Scope> getParent() = 0;

    virtual SharedPtr<VarDeclNode> resolveVariable(const String &name) = 0;

protected:
    SharedPtrVector<LocalScope> children;
    SharedPtrMap<String, VarDeclNode> variables;
};

class TopLevelScope : public Scope {
public:
    static SharedPtr<TopLevelScope> create();

    TopLevelScope() = default;

    ~TopLevelScope() override = default;

    bool isTopLevel() override;

    SharedPtr<TopLevelScope> getTopLevel() override;

    SharedPtr<Scope> getParent() override;

    virtual void addFunction(const String &name, const SharedPtr<FunctionDeclNode> &funcDecl) final;

    SharedPtr<VarDeclNode> resolveVariable(const String &name) override;

    SharedPtr<FunctionDeclNode> resolveFunction(const String &name);

protected:
    SharedPtrMap<String, FunctionDeclNode> functions;
};

class LocalScope : public Scope {
public:
    static SharedPtr<LocalScope> create(const SharedPtr<Scope> &parent);

    explicit LocalScope(const SharedPtr<Scope> &parent);

    ~LocalScope() override = default;

    bool isTopLevel() override;

    SharedPtr<TopLevelScope> getTopLevel() override;

    SharedPtr<Scope> getParent() override;

    SharedPtr<VarDeclNode> resolveVariable(const String &name) override;

protected:
    SharedPtr<Scope> parent;
};