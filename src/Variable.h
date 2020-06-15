#ifndef STATICSCRIPT_VARIABLE_H
#define STATICSCRIPT_VARIABLE_H

#include <antlr4-runtime.h>
#include "Type.h"
#include "Symbol.h"

class Scope;

class Variable : public Symbol {
public:
    Type *type = nullptr;

    bool defaultBoolVal = false;

    int defaultIntVal = 0;

    std::string defaultStringVal;

    Variable(const std::string &name, Scope *enclosingScope, antlr4::ParserRuleContext *context);
};

#endif // STATICSCRIPT_VARIABLE_H