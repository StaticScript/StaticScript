#include "Entity/Scope.h"

void Scope::addChild(const SharedPtr<LocalScope> &scope) {
    children.push_back(scope);
}

SharedPtr<TopLevelScope> TopLevelScope::getTopLevel() {
    return shared_from_this();
}

SharedPtr<Scope> TopLevelScope::getParent() {
    return nullptr;
}

SharedPtr<LocalScope> LocalScope::create(const SharedPtr<Scope> &parent) {
    const SharedPtr<LocalScope> &localScope = makeShared<LocalScope>(parent);
    parent->addChild(localScope);
    return localScope;
}

LocalScope::LocalScope(const SharedPtr<Scope> &parent) : parent(parent) {
}

bool LocalScope::isTopLevel() {
    return false;
}

SharedPtr<TopLevelScope> LocalScope::getTopLevel() {
    return parent->getTopLevel();
}

SharedPtr<Scope> LocalScope::getParent() {
    return parent;
}
