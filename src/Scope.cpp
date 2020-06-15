#include "Scope.h"

void Scope::addSymbol(Symbol *symbol) {
    if (symbol != nullptr) {
        symbols->push_back(symbol);
        symbol->enclosingScope = this;
    }
}

Variable *Scope::getVariable(Scope *scope, const std::string &name) {
    for (Symbol *symbol: *scope->symbols) {
        if (Variable *variable = dynamic_cast<Variable *>(symbol); variable != nullptr && symbol->name == name) {
            return variable;
        }
    }
    return nullptr;
}

Variable *Scope::getVariable(const std::string &name) {
    return getVariable(this, name);
}

bool Scope::containsSymbol(Symbol *symbol) {
    return std::find(symbols->begin(), symbols->end(), symbol) != symbols->end();
}
