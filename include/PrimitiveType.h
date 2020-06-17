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

    const std::string &getName() const override;

    Scope *getEnclosingScope() const override;

    bool isType(const Type *type) const override;

    const std::string &toString() const;

private:
    explicit PrimitiveType(std::string name);

    std::string name;
};

#endif // STATICSCRIPT_PRIMITIVETYPE_H