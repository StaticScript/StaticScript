#include <antlr4-runtime.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "AST/ASTBuilder.h"
#include "Sema/ScopeScanner.h"
#include "Sema/ReferenceResolver.h"
#include "Sema/TypeChecker.h"
#include "Sema/SemanticValidator.h"
#include "CodeGen/IRGenerator.h"
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
        // 语义分析: 1. 作用域扫描
        SharedPtr<ScopeScanner> scanner = makeShared<ScopeScanner>();
        scanner->resolve(module);
        // 语义分析: 2. 引用消解
        SharedPtr<ReferenceResolver> resolver = makeShared<ReferenceResolver>();
        resolver->resolve(module);
        // 语义分析: 3. 类型检查
        SharedPtr<TypeChecker> checker = makeShared<TypeChecker>();
        checker->resolve(module);
        // 语义分析: 4. 正确性检查
        SharedPtr<SemanticValidator> validator = makeShared<SemanticValidator>();
        validator->resolve(module);
        // 中间代码生成
        SharedPtr<IRGenerator> generator = makeShared<IRGenerator>();
        generator->resolve(module);

        LLVMModule &llvmModule = generator->getLLVMModule();

        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();

        const String &targetTriple = llvm::sys::getDefaultTargetTriple();
        llvmModule.setTargetTriple(targetTriple);

        String error;
        const llvm::Target *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

        if (!target) {
            llvm::errs() << error;
            return 1;
        }

        String cpu = "generic";
        String features;
        llvm::TargetOptions opt;
        auto rm = llvm::Optional<llvm::Reloc::Model>();
        llvm::TargetMachine *targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);

        llvmModule.setDataLayout(targetMachine->createDataLayout());

        String outputFilename = llvm::sys::path::stem(codeFilename).str() + ".bc";

        std::error_code ec;
        llvm::raw_fd_ostream dest(outputFilename, ec, llvm::sys::fs::OF_None);

        if (ec) {
            llvm::errs() << "Could not open file: " << ec.message();
        }

        llvm::WriteBitcodeToFile(llvmModule, dest);
        dest.flush();
    } else {
        throw DriverException("至少需要一个参数");
    }
}