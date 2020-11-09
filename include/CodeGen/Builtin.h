#pragma once

#include <llvm/Support/SourceMgr.h>
#include "config.h"
#include "Util/Alias.h"
#include <llvm/Linker/Linker.h>

struct Builtin {
    static void initialize(LLVMModule &destModule, LLVMContext &context);

    static inline LLVMType *stringType = nullptr;
    static inline LLVMType *stringPtrType = nullptr;
    static inline LLVMFunction *stringCreate = nullptr;
    static inline LLVMFunction *stringDelete = nullptr;
    static inline LLVMFunction *stringGetSize = nullptr;
    static inline LLVMFunction *stringAppend = nullptr;
    static inline LLVMFunction *stringPrepend = nullptr;
    static inline LLVMFunction *stringSlice = nullptr;
    static inline LLVMFunction *stringEquals = nullptr;
    static inline LLVMFunction *stringIndexOf = nullptr;
    static inline LLVMFunction *stringTrimLeft = nullptr;
    static inline LLVMFunction *stringTrimRight = nullptr;
    static inline LLVMFunction *stringTrim = nullptr;
};
