#ifndef STATICSCRIPT_SCOPE_H
#define STATICSCRIPT_SCOPE_H

#include <antlr4-runtime.h>
#include "Symbol.h"
#include "Scope.h"
#include "Variable.h"

class Scope : public Symbol {
public:
    std::vector<Symbol *> *symbols = new std::vector<Symbol *>();

    void addSymbol(Symbol *symbol);

    bool containsSymbol(Symbol *symbol);

    Variable *getVariable(const std::string &name);

    static Variable *getVariable(Scope *scope, const std::string &name);
};

#endif // STATICSCRIPT_SCOPE_H