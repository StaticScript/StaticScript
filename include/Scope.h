#ifndef STATICSCRIPT_SCOPE_H
#define STATICSCRIPT_SCOPE_H

#include <antlr4-runtime.h>
#include "Symbol.h"
#include "Scope.h"
#include "Variable.h"

class Function;

class Scope : public Symbol {
public:
    std::vector<Symbol *> *symbols = new std::vector<Symbol *>();

    void addSymbol(Symbol *symbol);

    bool containsSymbol(const Symbol *symbol) const;

    Variable *getVariable(const std::string &name) const;

    Function *getFunction(const std::string &name, const std::vector<Type *> *paramTypes) const;

    static Variable *getVariable(const Scope *scope, const std::string &name);

    static Function *getFunction(const Scope *scope, const std::string &name, const std::vector<Type *> *paramTypes);
};

#endif // STATICSCRIPT_SCOPE_H