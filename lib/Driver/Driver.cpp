#include <iostream>
#include <antlr4-runtime.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "Driver/Visitor.h"

int main(int argc, char *argv[]) {
    antlr4::ANTLRInputStream inputStream("let x = 1; const y = x + 1;");
    StaticScriptLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    tokenStream.fill();
    StaticScriptParser parser(&tokenStream);
    antlr4::tree::ParseTree *tree = parser.module();
    Visitor visitor("code.ss");
    SharedPtr<Module> module = visitor.visit(tree);
    std::cout << module->getFilename() << std::endl;
}