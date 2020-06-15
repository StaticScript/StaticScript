#include "Variable.h"
#include "Scope.h"

Variable::Variable(const std::string &name, Scope *enclosingScope, antlr4::ParserRuleContext *context) {
    this->name = name;
    this->enclosingScope = enclosingScope;
    this->context = context;
}