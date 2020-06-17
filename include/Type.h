#ifndef STATICSCRIPT_TYPE_H
#define STATICSCRIPT_TYPE_H

#include <antlr4-runtime.h>

class Scope;

class Type {
public:
    virtual const std::string &getName() const = 0;

    virtual Scope *getEnclosingScope() const = 0;

    virtual bool isType(const Type *type) const = 0;
};

#endif //STATICSCRIPT_TYPE_H
