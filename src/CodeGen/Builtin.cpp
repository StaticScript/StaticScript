#include "CodeGen/Builtin.h"

void Builtin::initialize(LLVMModule &module, LLVMContext &context, const llvm::Twine &libDirArg) {
    llvm::SmallString<256> libPath;
    if (!libDirArg.str().empty()) {
        libDirArg.toVector(libPath);
        if (!llvm::sys::path::is_absolute(libPath)) {
            if (llvm::sys::fs::make_absolute(libPath)) {
                reportLinkError("Parse the specified library directory failed");
            }
        }
        if (!llvm::sys::fs::exists(libPath)) {
            reportLinkError("The specified library directory does not exist");
        }
        if (!llvm::sys::fs::is_directory(libPath)) {
            reportLinkError("The specified library directory is not a directory");
        }
        libDir = libPath.str();
    } else {
        llvm::sys::fs::current_path(libPath);
        llvm::sys::path::append(libPath, "lib");
        if (llvm::sys::fs::exists(libPath) && llvm::sys::fs::is_directory(libPath)) {
            libDir = libPath.str();
        }
    }

    BuiltinError::linkModule(module, context);
    BuiltinString::linkModule(module, context);
    BuiltinArray::linkModule(module, context);
    BuiltinIO::linkModule(module, context);

    BuiltinError::getTypeAndFunction(module);
    BuiltinString::getTypeAndFunction(module);
    BuiltinArray::getTypeAndFunction(module);
    BuiltinIO::getTypeAndFunction(module);
}

#define BUILTIN_LINK_MODULE(moduleName) \
    llvm::SMDiagnostic error; \
    String bitcodeFilename = Builtin::libDir + "/ss_"#moduleName".bc"; \
    bool bitcodeExist = llvm::sys::fs::exists(bitcodeFilename); \
    reportOnLinkError(!bitcodeExist, "Not found " + bitcodeFilename); \
    std::unique_ptr<LLVMModule> bitcodeModule = llvm::parseIRFile(bitcodeFilename, error, context); \
    reportOnLinkError(!bitcodeModule, "Parse bitcode file of the "#moduleName" module failed"); \
    bool linkFailed = llvm::Linker::linkModules(module, std::move(bitcodeModule)); \
    reportOnLinkError(linkFailed, "Link "#moduleName" module failed");

void BuiltinError::linkModule(LLVMModule &module, LLVMContext &context) {
    BUILTIN_LINK_MODULE(error)
}

void BuiltinError::getTypeAndFunction(LLVMModule &module) {
    llvm::StructType *errStructType = module.getTypeByName("struct.ss_error");
    type = errStructType->getPointerTo();
    exitIfErrorFunc = module.getFunction("ss_exit_if_error");
    assertFunc = module.getFunction("ss_assert");
}

void BuiltinString::linkModule(LLVMModule &module, LLVMContext &context) {
    BUILTIN_LINK_MODULE(string)
}

void BuiltinString::getTypeAndFunction(LLVMModule &module) {
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

void BuiltinArray::linkModule(LLVMModule &module, LLVMContext &context) {
    BUILTIN_LINK_MODULE(array)
}

void BuiltinArray::getTypeAndFunction(LLVMModule &module) {
    llvm::StructType *arrStructType = module.getTypeByName("struct.ss_array");
    type = arrStructType->getPointerTo();
    createIntegerArrayFunc = module.getFunction("ss_array_create_integer_array");
    createBooleanArrayFunc = module.getFunction("ss_array_create_boolean_array");
    createStringArrayFunc = module.getFunction("ss_array_create_string_array");
    createArrayArrayFunc = module.getFunction("ss_array_create_array_array");
    createIntegerArrayWithLiteralFunc = module.getFunction("ss_array_create_integer_array_with_literal");
    createBooleanArrayWithLiteralFunc = module.getFunction("ss_array_create_boolean_array_with_literal");
    createStringArrayWithLiteralFunc = module.getFunction("ss_array_create_string_array_with_literal");
    createArrayArrayWithLiteralFunc = module.getFunction("ss_array_create_array_array_with_literal");
    deleteFunc = module.getFunction("ss_array_delete");
    getSizeFunc = module.getFunction("ss_array_get_size");
    isNDArrayFunc = module.getFunction("ss_array_is_nd_array");
    pushIntegerFunc = module.getFunction("ss_array_push_integer");
    pushBooleanFunc = module.getFunction("ss_array_push_boolean");
    pushStringFunc = module.getFunction("ss_array_push_string");
    pushArrayFunc = module.getFunction("ss_array_push_array");
    popIntegerFunc = module.getFunction("ss_array_pop_integer");
    popBooleanFunc = module.getFunction("ss_array_pop_boolean");
    popStringFunc = module.getFunction("ss_array_pop_string");
    popArrayFunc = module.getFunction("ss_array_pop_array");
    getIntegerFunc = module.getFunction("ss_array_get_integer");
    getBooleanFunc = module.getFunction("ss_array_get_boolean");
    getStringFunc = module.getFunction("ss_array_get_string");
    getArrayFunc = module.getFunction("ss_array_get_array");
    setIntegerFunc = module.getFunction("ss_array_set_integer");
    setBooleanFunc = module.getFunction("ss_array_set_boolean");
    setStringFunc = module.getFunction("ss_array_set_string");
    setArrayFunc = module.getFunction("ss_array_set_array");
    sliceFunc = module.getFunction("ss_array_slice");
}

void BuiltinIO::linkModule(LLVMModule &module, LLVMContext &context) {
    BUILTIN_LINK_MODULE(io)
}

void BuiltinIO::getTypeAndFunction(LLVMModule &module) {
    integer2stringFunc = module.getFunction("ss_integer2string");
    string2integerFunc = module.getFunction("ss_string2integer");
    printIntegerFunc = module.getFunction("ss_print_integer");
    printlnIntegerFunc = module.getFunction("ss_println_integer");
    printStringFunc = module.getFunction("ss_print_string");
    printlnStringFunc = module.getFunction("ss_println_string");
}