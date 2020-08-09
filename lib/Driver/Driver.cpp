#include <iostream>
#include <antlr4-runtime.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "Driver/Visitor.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        const String &codeFilename = argv[1];
        std::ifstream fin(codeFilename);
        if (!fin) {
            std::cerr << "Can not open " << argv[1] << '.' << std::endl;
            return 1;
        }
        antlr4::ANTLRInputStream inputStream(fin);
        StaticScriptLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        tokenStream.fill();
        StaticScriptParser parser(&tokenStream);
        antlr4::tree::ParseTree *tree = parser.module();
        Visitor visitor(codeFilename);
        SharedPtr<Module> module = visitor.visit(tree);
        std::cout << module->getFilename() << std::endl;
    } else {
        std::cerr << "At least one parameter is required." << std::endl;
    }
}