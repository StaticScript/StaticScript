#include "Symbol.h"
#include "Scope.h"

const std::string &Symbol::getName() const {
    return name;
}

Scope *Symbol::getEnclosingScope() const {
    return enclosingScope;
}
