#ifndef STATICSCRIPT_FUNCTIONTYPE_H
#define STATICSCRIPT_FUNCTIONTYPE_H

#include <antlr4-runtime.h>
#include "Type.h"

class FunctionType : public Type {
public:
    virtual Type *getReturnType() = 0;

    virtual std::vector<Type *> *getParamTypes() = 0;

    virtual bool matchParameterTypes(std::vector<Type *> *paramTypes) = 0;
};

#endif //STATICSCRIPT_FUNCTIONTYPE_H
