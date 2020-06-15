#include <iostream>
#include <antlr4-runtime.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "StaticScriptHeaders.h"

using namespace std;
using namespace antlr4;

int main(int argc, char *argv[]) {
    ANTLRInputStream inputStream("let x = 1, y = 2; if (y > 2) { x = y; }");
    StaticScriptLexer lexer(&inputStream);
    CommonTokenStream tokenStream(&lexer);
    tokenStream.fill();
    StaticScriptParser parser(&tokenStream);
    tree::ParseTree *tree = parser.module();
    cout << tree->toStringTree(&parser, true) << endl;
}