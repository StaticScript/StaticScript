#ifndef STATICSCRIPT_SYMBOL_H
#define STATICSCRIPT_SYMBOL_H

#include <antlr4-runtime.h>

class Scope;

class Symbol {
public:
    std::string name;

    int visibility = 0;

    antlr4::ParserRuleContext *context = nullptr;

    Scope *enclosingScope = nullptr;

    virtual const std::string &getName() const;

    virtual Scope *getEnclosingScope() const;
};

#endif // STATICSCRIPT_SYMBOL_H