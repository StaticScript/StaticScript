#pragma once

#include <memory>
#include "Util/Alias.h"

class TopLevelScope;

class LocalScope;

class Scope {
public:
    Scope() = default;

    virtual ~Scope() = default;

    virtual void addChild(const SharedPtr<LocalScope> &scope);

    virtual bool isTopLevel() = 0;

    virtual SharedPtr<TopLevelScope> getTopLevel() = 0;

    virtual SharedPtr<Scope> getParent() = 0;

protected:
    SharedPtrVector<LocalScope> children;
};

class TopLevelScope : public Scope, public std::enable_shared_from_this<TopLevelScope> {
public:
    static SharedPtr<TopLevelScope> create() {
        return makeShared<TopLevelScope>();
    }

    TopLevelScope() = default;

    ~TopLevelScope() override = default;

    bool isTopLevel() override {
        return true;
    }

    SharedPtr<TopLevelScope> getTopLevel() override;

    SharedPtr<Scope> getParent() override;
};

class LocalScope : public Scope, public std::enable_shared_from_this<LocalScope> {
public:
    static SharedPtr<LocalScope> create(const SharedPtr<Scope> &parent);

    explicit LocalScope(const SharedPtr<Scope> &parent);

    ~LocalScope() override = default;

    bool isTopLevel() override;

    SharedPtr<TopLevelScope> getTopLevel() override;

    SharedPtr<Scope> getParent() override;

protected:
    SharedPtr<Scope> parent;
};