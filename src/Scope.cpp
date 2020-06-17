#include "Scope.h"
#include "Function.h"

void Scope::addSymbol(Symbol *symbol) {
    if (symbol != nullptr) {
        symbols->push_back(symbol);
        symbol->enclosingScope = this;
    }
}

bool Scope::containsSymbol(const Symbol *symbol) const {
    return std::find(symbols->begin(), symbols->end(), symbol) != symbols->end();
}

Variable *Scope::getVariable(const std::string &name) const {
    return getVariable(this, name);
}

Variable *Scope::getVariable(const Scope *scope, const std::string &name) {
    for (Symbol *symbol: *scope->symbols) {
        if (Variable *variable = dynamic_cast<Variable *>(symbol); variable != nullptr && symbol->name == name) {
            return variable;
        }
    }
    return nullptr;
}

Function *Scope::getFunction(const std::string &name, const std::vector<Type *> *paramTypes) const {
    return getFunction(this, name, paramTypes);
}

Function *Scope::getFunction(const Scope *scope, const std::string &name, const std::vector<Type *> *paramTypes) {
    for (Symbol *symbol: *scope->symbols) {
        if (Function *function = dynamic_cast<Function *>(symbol); function != nullptr && symbol->name == name) {
            if (function->matchParameterTypes(paramTypes)) {
                return function;
            }
        }
    }
    return nullptr;
}