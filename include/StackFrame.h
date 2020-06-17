#ifndef STATICSCRIPT_STACKFRAME_H
#define STATICSCRIPT_STACKFRAME_H

#include <antlr4-runtime.h>
#include "Scope.h"

class StackFrame {
public:
    Scope *scope = nullptr;

    StackFrame *parentFrame = nullptr;
};


#endif //STATICSCRIPT_STACKFRAME_H
