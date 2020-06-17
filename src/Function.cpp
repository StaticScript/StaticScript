#include "Function.h"

Function::Function(const std::string &name, Scope *enclosingScope, antlr4::ParserRuleContext *context) {
    this->name = name;
    this->enclosingScope = enclosingScope;
    this->context = context;
}

Type *Function::getReturnType() const {
    return returnType;
}

std::vector<Type *> *Function::getParamTypes() {
    if (paramTypes == nullptr) {
        paramTypes = new std::vector<Type *>();
    }
    // 这里是不是应该放在if语句里面?
    for (Variable *param : *parameters) {
        paramTypes->push_back(param->type);
    }
    return paramTypes;
}

bool Function::matchParameterTypes(const std::vector<Type *> *externalParamTypes) const {
    if (parameters->size() != externalParamTypes->size()) {
        return false;
    }
    bool matched = true;
    for (int i = 0; i < externalParamTypes->size(); i++) {
        Variable *parameter = parameters->at(i);
        Type *type = externalParamTypes->at(i);
        if (!parameter->type->isType(type)) {
            matched = false;
            break;
        }
    }
    return matched;
}