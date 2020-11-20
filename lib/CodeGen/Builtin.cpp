#include "CodeGen/Builtin.h"
#include "Support/Exception.h"

void BuiltinString::initialize(LLVMModule &module, LLVMContext &context) {
    llvm::SMDiagnostic error;
    String stringBitcodeFilename = PROJECT_BINARY_DIR"/builtin/ss_string.bc";
    if (!llvm::sys::fs::exists(stringBitcodeFilename)) {
        throw CodeGenException("Not found " + stringBitcodeFilename);
    }
    std::unique_ptr<LLVMModule> stringModule = llvm::parseIRFile(stringBitcodeFilename, error, context);
    if (!stringModule) {
        throw CodeGenException("Parse bitcode file of the string module failed");
    }
    if (llvm::Linker::linkModules(module, std::move(stringModule))) {
        throw CodeGenException("Link string module failed");
    }
    llvm::StructType *strStructType = module.getTypeByName("struct.ss_string");
    type = strStructType->getPointerTo();
    createFunc = module.getFunction("ss_string_create");
    deleteFunc = module.getFunction("ss_string_delete");
    getSizeFunc = module.getFunction("ss_string_get_size");
    concatFunc = module.getFunction("ss_string_concat");
    appendFunc = module.getFunction("ss_string_append");
    prependFunc = module.getFunction("ss_string_prepend");
    sliceFunc = module.getFunction("ss_string_slice");
    equalsFunc = module.getFunction("ss_string_equals");
    indexOfFunc = module.getFunction("ss_string_index_of");
    trimLeftFunc = module.getFunction("ss_string_trim_left");
    trimRightFunc = module.getFunction("ss_string_trim_right");
    trimFunc = module.getFunction("ss_string_trim");
}

void BuiltinIO::initialize(LLVMModule &module, LLVMContext &context) {
    llvm::SMDiagnostic error;
    String ioBitcodeFilename = PROJECT_BINARY_DIR"/builtin/ss_io.bc";
    if (!llvm::sys::fs::exists(ioBitcodeFilename)) {
        throw CodeGenException("Not found " + ioBitcodeFilename);
    }
    std::unique_ptr<LLVMModule> ioModule = llvm::parseIRFile(ioBitcodeFilename, error, context);
    if (!ioModule) {
        throw CodeGenException("Parse bitcode file of the io module failed");
    }
    if (llvm::Linker::linkModules(module, std::move(ioModule))) {
        throw CodeGenException("Link io module failed");
    }
    integer2stringFunc = module.getFunction("ss_integer2string");
    string2integerFunc = module.getFunction("ss_string2integer");
    printIntegerFunc = module.getFunction("ss_print_integer");
    printlnIntegerFunc = module.getFunction("ss_println_integer");
    printStringFunc = module.getFunction("ss_print_string");
    printlnStringFunc = module.getFunction("ss_println_string");
}

void Builtin::initialize(LLVMModule &module, LLVMContext &context) {
    BuiltinString::initialize(module, context);
    BuiltinIO::initialize(module, context);
}
