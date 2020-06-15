#ifndef STATICSCRIPT_ANNOTATEDTREE_H
#define STATICSCRIPT_ANNOTATEDTREE_H

#include <antlr4-runtime.h>
#include "Type.h"
#include "Symbol.h"
#include "Scope.h"
#include "Variable.h"

class AnnotatedTree {
public:
    // AST
    antlr4::tree::ParseTree *ast = nullptr;
    // 解析出来的所有类型
    std::vector<Type *> *types = new std::vector<Type *>();
    // AST节点对应的Symbol
    std::map<antlr4::ParserRuleContext *, Symbol *> *symbolOfNode = new std::map<antlr4::ParserRuleContext *, Symbol *>();
    // AST节点对应的Scope，如{}、函数调用会启动新的Scope
    std::map<antlr4::ParserRuleContext *, Scope *> *node2Scope = new std::map<antlr4::ParserRuleContext *, Scope *>();
    // 用于做类型推断，每个节点推断出来的类型
    std::map<antlr4::ParserRuleContext *, Type *> *typeOfNode = new std::map<antlr4::ParserRuleContext *, Type *>();

    AnnotatedTree();

    Variable *lookupVariable(Scope *scope, const std::string &idName);
};


#endif //STATICSCRIPT_ANNOTATEDTREE_H
