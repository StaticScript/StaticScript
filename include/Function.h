#ifndef STATICSCRIPT_FUNCTION_H
#define STATICSCRIPT_FUNCTION_H

#include <antlr4-runtime.h>
#include "Scope.h"
#include "FunctionType.h"
#include "Variable.h"

class Function : public Scope, public FunctionType {
public:
    // 形参列表
    std::vector<Variable *> *parameters = new std::vector<Variable *>();

    // 返回类型
    Type *returnType = nullptr;

    // 闭包变量(所引用的外部变量)
    std::set<Variable *> *closureVariables = nullptr;

    // 构造函数
    Function(const std::string &name, Scope *enclosingScope, antlr4::ParserRuleContext *context);

    // 获取返回类型
    Type *getReturnType() const override;

    // 获取形参类型列表
    std::vector<Type *> *getParamTypes() override;

    // 匹配形参类型列表
    bool matchParameterTypes(const std::vector<Type *> *externalParamTypes) const override;

private:
    // 形参类型列表
    std::vector<Type *> *paramTypes = nullptr;
};

#endif // STATICSCRIPT_FUNCTION_H