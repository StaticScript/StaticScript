#include "BlockScope.h"

BlockScope::BlockScope() {
    this->name = "block" + std::to_string(index++);
}

BlockScope::BlockScope(Scope *enclosingScope, antlr4::ParserRuleContext *context) {
    this->name = "block" + std::to_string(index++);
    this->enclosingScope = enclosingScope;
    this->context = context;
}
