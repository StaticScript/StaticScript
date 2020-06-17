#ifndef STATICSCRIPT_BLOCKSCOPE_H
#define STATICSCRIPT_BLOCKSCOPE_H

#include <antlr4-runtime.h>
#include "Scope.h"

class BlockScope : public Scope {
public:
    BlockScope();

    BlockScope(Scope *enclosingScope, antlr4::ParserRuleContext *context);

private:
    inline static int index = 1;
};

#endif // STATICSCRIPT_BLOCKSCOPE_H