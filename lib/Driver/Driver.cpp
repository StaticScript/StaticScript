#include <antlr4-runtime.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "Sema/ScopeScanner.h"
#include "Sema/ReferenceResolver.h"
#include "Sema/TypeChecker.h"
#include "Sema/SemanticValidator.h"
#include "Exception/DriverException.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        const String &codeFilename = argv[1];
        std::ifstream fin(codeFilename);
        if (!fin) {
            throw DriverException("打开文件失败");
        }
        antlr4::ANTLRInputStream inputStream(fin);
        StaticScriptLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokenStream(&lexer);
        tokenStream.fill();
        StaticScriptParser parser(&tokenStream);
        antlr4::tree::ParseTree *tree = parser.module();
        ASTBuilder builder(codeFilename);
        SharedPtr<ModuleNode> module = builder.visit(tree);
        SharedPtr<ScopeScanner> scanner = makeShared<ScopeScanner>();
        scanner->resolve(module);
        SharedPtr<ReferenceResolver> resolver = makeShared<ReferenceResolver>();
        resolver->resolve(module);
        SharedPtr<TypeChecker> checker = makeShared<TypeChecker>();
        checker->resolve(module);
        SharedPtr<SemanticValidator> validator = makeShared<SemanticValidator>();
        validator->resolve(module);
    } else {
        throw DriverException("至少需要一个参数");
    }
}