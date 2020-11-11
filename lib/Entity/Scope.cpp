#include "Entity/Scope.h"

void Scope::addChild(const SharedPtr<LocalScope> &scope) {
    children.push_back(scope);
}

void Scope::addVariable(const String &varName, const SharedPtr<VarDeclNode> &varDecl) {
    variables[varName] = varDecl;
}

bool Scope::hasVariable(const String &varName) {
    return mapFind(variables, varName) != nullptr;
}

SharedPtr<TopLevelScope> TopLevelScope::create() {
    return makeShared<TopLevelScope>();
}

bool TopLevelScope::isTopLevel() {
    return true;
}

SharedPtr<Scope> TopLevelScope::getParent() {
    return nullptr;
}

SharedPtr<TopLevelScope> TopLevelScope::getTopLevel() {
    return staticPtrCast<TopLevelScope>(shared_from_this());
}

void TopLevelScope::addFunction(const String &funcName, const SharedPtr<FunctionDeclNode> &funcDecl) {
    functions[funcName] = funcDecl;
}

SharedPtr<VarDeclNode> TopLevelScope::resolveVariable(const String &name) {
    return mapFind(variables, name);
}

SharedPtr<FunctionDeclNode> TopLevelScope::resolveFunction(const String &name) {
    SharedPtr<FunctionDeclNode> function = mapFind(functions, name);
    if (!function) {
        return mapFind(builtinFunctions, name);
    }
    return function;
}

bool TopLevelScope::hasFunction(const String &name) {
    return resolveFunction(name) != nullptr;
}

SharedPtr<LocalScope> LocalScope::create(const SharedPtr<Scope> &parent) {
    const SharedPtr<LocalScope> &localScope = makeShared<LocalScope>(parent);
    parent->addChild(localScope);
    return localScope;
}

LocalScope::LocalScope(const SharedPtr<Scope> &parent) : parent(parent) {}

bool LocalScope::isTopLevel() {
    return false;
}

SharedPtr<TopLevelScope> LocalScope::getTopLevel() {
    return parent->getTopLevel();
}

SharedPtr<Scope> LocalScope::getParent() {
    return parent;
}

SharedPtr<VarDeclNode> LocalScope::resolveVariable(const String &name) {
    auto variable = mapFind(variables, name);
    return variable ? variable : parent->resolveVariable(name);
}
