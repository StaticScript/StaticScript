#include <antlr4-runtime.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include "StaticScriptLexer.h"
#include "StaticScriptParser.h"
#include "AST/ASTBuilder.h"
#include "Sema/ScopeScanner.h"
#include "Sema/ReferenceResolver.h"
#include "Sema/TypeChecker.h"
#include "Sema/SemanticValidator.h"
#include "CodeGen/IRGenerator.h"
#include "Exception/DriverException.h"

int drive(int argc, char **argv) {
    llvm::cl::getRegisteredOptions().clear();
    llvm::cl::OptionCategory generalOptsCat("General Options");
    llvm::cl::OptionCategory genericOptsCat("Generic Options");
    llvm::cl::opt<String> inputFilename(llvm::cl::Positional, llvm::cl::desc("<input file>"));
    llvm::cl::opt<String> outputFilename("o", llvm::cl::value_desc("output file"), llvm::cl::desc("Write output to <output file>"), llvm::cl::cat(generalOptsCat));
    llvm::cl::opt<bool> emitLLVM("emit-llvm", llvm::cl::desc("Generate the LLVM representation for input file"), llvm::cl::cat(generalOptsCat));
    llvm::cl::opt<bool> showHelp("help", llvm::cl::desc("Display available options"), llvm::cl::cat(genericOptsCat));
    llvm::cl::opt<bool> showHelpAlias("h", llvm::cl::desc("Alias for --help"), llvm::cl::cat(genericOptsCat));
    llvm::cl::opt<bool> showVersion("version", llvm::cl::desc("Display the version of this program"), llvm::cl::cat(genericOptsCat));
    llvm::cl::opt<bool> showVersionAlias("v", llvm::cl::desc("Alias for --version"), llvm::cl::cat(genericOptsCat));

    llvm::cl::ParseCommandLineOptions(argc, argv, "StaticScript Compiler\n");

    if (showHelp || showHelpAlias) {
        llvm::cl::PrintHelpMessage(false, true);
        return 0;
    }

    if (showVersion || showVersionAlias) {
        llvm::outs() << "StaticScript v" << PROJECT_VERSION << '\n';
        return 0;
    }

    if (inputFilename.empty()) {
        llvm::cl::PrintHelpMessage(false, true);
        return 0;
    }

    if (outputFilename.empty()) {
        outputFilename.setValue(emitLLVM ? "ss-ir.ll" : "ss-out.o");
    }

    if(!llvm::sys::fs::exists(inputFilename)) {
        throw DriverException("Not found input file");
    }
    std::ifstream fin(inputFilename.data());
    if (!fin) {
        throw DriverException("Open input file failed");
    }

    antlr4::ANTLRInputStream inputStream(fin);
    StaticScriptLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    tokenStream.fill();
    StaticScriptParser parser(&tokenStream);
    antlr4::tree::ParseTree *tree = parser.module();
    ASTBuilder builder(inputFilename.data());
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

    String targetErrorMsg;
    const llvm::Target *target = llvm::TargetRegistry::lookupTarget(targetTriple, targetErrorMsg);

    if (!target) {
        throw DriverException(targetErrorMsg);
    }

    String cpu = "generic";
    String features;
    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    llvm::TargetMachine *targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);

    llvmModule.setDataLayout(targetMachine->createDataLayout());

    std::error_code ec;
    llvm::raw_fd_ostream dest(outputFilename, ec, llvm::sys::fs::OF_None);

    if (ec) {
        throw DriverException("Could not create file: " + ec.message());
    }

    if (emitLLVM) {
        dest << llvmModule;
    } else {
        llvm::legacy::PassManager pass;
        auto fileType = llvm::CGFT_ObjectFile;
        if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
            throw DriverException("TargetMachine can't emit a object file");
        }
        pass.run(llvmModule);
    }
    dest.flush();
    dest.close();
    fin.close();
    return 0;
}

int main(int argc, char *argv[]) {
    try {
        return drive(argc, argv);
    } catch (const SemanticException &e) {
        llvm::errs() << "[Semantic Exception] " << e.what() << '\n';
    } catch (const CodeGenException &e) {
        llvm::errs() << "[CodeGen Exception] " << e.what() << '\n';
    } catch (const DriverException &e) {
        llvm::errs() << "[Driver Exception] " << e.what() << '\n';
    } catch (const std::exception &e) {
        llvm::errs() << "[Unknown Exception] " << e.what() << '\n';
    }
}
