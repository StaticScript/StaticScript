#ifndef STATICSCRIPT_ANNOTATEDTREE_H
#define STATICSCRIPT_ANNOTATEDTREE_H

#include <antlr4-runtime.h>
#include "Type.h"
#include "Symbol.h"
#include "Scope.h"
#include "Variable.h"
#include "Function.h"
#include "StaticScriptParser.h"

class AnnotatedTree {
public:
    typedef std::map<antlr4::ParserRuleContext *, Symbol *> MapForContextAndSymbol;
    typedef std::map<antlr4::ParserRuleContext *, Scope *> MapForContextAndScope;
    typedef std::map<antlr4::ParserRuleContext *, Type *> MapForContextAndType;
    // AST
    antlr4::tree::ParseTree *ast = nullptr;
    // 解析出来的所有类型
    std::vector<Type *> *types = new std::vector<Type *>();
    // AST节点对应的Symbol
    MapForContextAndSymbol *symbolOfNode = new MapForContextAndSymbol();
    // AST节点对应的Scope，如{}、函数调用会启动新的Scope
    MapForContextAndScope *node2Scope = new MapForContextAndScope();
    // 用于做类型推断，每个节点推断出来的类型
    MapForContextAndType *typeOfNode = new MapForContextAndType();

    // 构造函数
    explicit AnnotatedTree();

    // 根据名称查找类型
    Type *lookupType(const std::string &idName) const;

    // 通过名称逐级查找变量
    Variable *lookupVariable(const Scope *scope, const std::string &idName) const;

    // 通过名称和签名查找函数
    Function *lookupFunction(const Scope *scope, const std::string &idName, const std::vector<Type *> *paramTypes) const;

    Scope *enclosingScopeOfNode(antlr4::ParserRuleContext *node) const;

    Function *enclosingFunctionOfNode(antlr4::RuleContext *context) const;
};


#endif //STATICSCRIPT_ANNOTATEDTREE_H
