#include "CodeGen/Builtin.h"
#include <iostream>

void Builtin::initialize(LLVMModule &destModule, LLVMContext &context) {
    llvm::SMDiagnostic error;
    std::unique_ptr<LLVMModule> module = llvm::parseIRFile(PROJECT_BINARY_DIR"/external/ss_string.bc", error, context);
    llvm::Linker::linkModules(destModule, std::move(module));
    stringType = destModule.getTypeByName("struct.ss_string");
    stringPtrType = stringType->getPointerTo();
    stringCreate = destModule.getFunction("ss_string_create");
    stringDelete = destModule.getFunction("ss_string_delete");
    stringGetSize = destModule.getFunction("ss_string_get_size");
    stringAppend = destModule.getFunction("ss_string_append");
    stringPrepend = destModule.getFunction("ss_string_prepend");
    stringSlice = destModule.getFunction("ss_string_slice");
    stringEquals = destModule.getFunction("ss_string_equals");
    stringIndexOf = destModule.getFunction("ss_string_index_of");
    stringTrimLeft = destModule.getFunction("ss_string_trim_left");
    stringTrimRight = destModule.getFunction("ss_string_trim_right");
    stringTrim = destModule.getFunction("ss_string_trim");
}
