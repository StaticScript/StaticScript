#ifndef STATICSCRIPT_AST_BASE_H
#define STATICSCRIPT_AST_BASE_H

#include "Util/Alias.h"

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void codegen() = 0;
};


#endif //STATICSCRIPT_AST_BASE_H
