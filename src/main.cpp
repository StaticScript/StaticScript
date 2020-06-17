#include <iostream>
#include <antlr4-runtime.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "StaticScriptHeaders.h"

using namespace std;

int main(int argc, char *argv[]) {
    antlr4::ANTLRInputStream inputStream("let x = 1, y = 2; if (y > 2) { x = y; }");
    StaticScriptLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    tokenStream.fill();
    StaticScriptParser parser(&tokenStream);
    antlr4::tree::ParseTree *tree = parser.module();
    Visitor visitor;
    visitor.visit(tree);
}