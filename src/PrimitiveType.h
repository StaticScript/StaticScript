#ifndef STATICSCRIPT_PRIMITIVETYPE_H
#define STATICSCRIPT_PRIMITIVETYPE_H

#include <antlr4-runtime.h>
#include "Type.h"
#include "Scope.h"

class PrimitiveType : public Type {
public:
    static PrimitiveType *Boolean;
    static PrimitiveType *Number;
    static PrimitiveType *String;

    const std::string &getName() override;

    Scope *getEnclosingScope() override;

    bool isType(Type *type) override;

    const std::string &toString();

private:
    explicit PrimitiveType(std::string name);

    std::string name;
};

#endif // STATICSCRIPT_PRIMITIVETYPE_H