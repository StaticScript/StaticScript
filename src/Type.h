#ifndef STATICSCRIPT_TYPE_H
#define STATICSCRIPT_TYPE_H

#include <antlr4-runtime.h>

class Scope;

class Type {
public:
    virtual const std::string &getName() = 0;

    virtual Scope *getEnclosingScope() = 0;

    virtual bool isType(Type *type) = 0;
};

#endif //STATICSCRIPT_TYPE_H
