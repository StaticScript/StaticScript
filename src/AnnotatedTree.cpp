#include "AnnotatedTree.h"

AnnotatedTree::AnnotatedTree() = default;

Variable *AnnotatedTree::lookupVariable(Scope *scope, const std::string &idName) {
    Variable *variable = scope->getVariable(idName);
    if (variable == nullptr && scope->enclosingScope != nullptr) {
        variable = lookupVariable(scope->enclosingScope, idName);
    }
    return variable;
}
