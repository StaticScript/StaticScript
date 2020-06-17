#include "AnnotatedTree.h"

AnnotatedTree::AnnotatedTree() = default;

Type *AnnotatedTree::lookupType(const std::string &idName) const {
    for (Type *type:*types) {
        if (type->getName() == idName) {
            return type;
        }
    }
    return nullptr;
}

Variable *AnnotatedTree::lookupVariable(const Scope *scope, const std::string &idName) const {
    Variable *variable = scope->getVariable(idName);
    if (variable == nullptr && scope->enclosingScope != nullptr) {
        variable = lookupVariable(scope->enclosingScope, idName);
    }
    return variable;
}

Function *AnnotatedTree::lookupFunction(const Scope *scope, const std::string &idName, const std::vector<Type *> *paramTypes) const {
    Function *function = scope->getFunction(idName, paramTypes);
    if (function != nullptr && scope->enclosingScope != nullptr) {
        function = lookupFunction(scope->enclosingScope, idName, paramTypes);
    }
    return function;
}

Scope *AnnotatedTree::enclosingScopeOfNode(antlr4::ParserRuleContext *node) const {
    Scope *scope = nullptr;
    if (antlr4::ParserRuleContext *parent = dynamic_cast<antlr4::ParserRuleContext *>(node->parent); parent != nullptr) {
        if (MapForContextAndScope::iterator it = node2Scope->find(parent); it != node2Scope->end() && it->second != nullptr) {
            scope = enclosingScopeOfNode(parent);
        }
    }
    return scope;
}

Function *AnnotatedTree::enclosingFunctionOfNode(antlr4::RuleContext *context) const {
    if (context != nullptr) {
        antlr4::tree::ParseTree *parent = context->parent;
        if (dynamic_cast<StaticScriptParser::FunctionDeclarationContext *>(parent) != nullptr) {
            if (antlr4::ParserRuleContext *parentContext = dynamic_cast<antlr4::ParserRuleContext *>(parent); parentContext != nullptr) {
                if (MapForContextAndScope::iterator it = node2Scope->find(parentContext); it != node2Scope->end()) {
                    return dynamic_cast<Function *>(it->second);
                } else {
                    return nullptr;
                }
            } else {
                return nullptr;
            }
        } else if (parent == nullptr) {
            return nullptr;
        } else {
            return enclosingFunctionOfNode(dynamic_cast<antlr4::RuleContext *>(parent));
        }
    } else {
        return nullptr;
    }
}
