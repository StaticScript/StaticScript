#ifndef STATICSCRIPT_FUNCTIONTYPE_H
#define STATICSCRIPT_FUNCTIONTYPE_H

#include <antlr4-runtime.h>
#include "Type.h"

class FunctionType : public Type {
public:
    virtual Type *getReturnType() const = 0;

    virtual std::vector<Type *> *getParamTypes() = 0;

    virtual bool matchParameterTypes(const std::vector<Type *> *paramTypes) const = 0;
};

#endif //STATICSCRIPT_FUNCTIONTYPE_H
