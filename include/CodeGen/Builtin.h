#pragma once

#include <llvm/Support/SourceMgr.h>
#include "config.h"
#include "Support/Alias.h"
#include <llvm/Linker/Linker.h>


class Builtin {
public:
    static void initialize(LLVMModule &module, LLVMContext &context);
};

class BuiltinString {
public:
    static void initialize(LLVMModule &module, LLVMContext &context);

    static inline LLVMType *type = nullptr;
    static inline LLVMFunction *createFunc = nullptr;
    static inline LLVMFunction *deleteFunc = nullptr;
    static inline LLVMFunction *getSizeFunc = nullptr;
    static inline LLVMFunction *concatFunc = nullptr;
    static inline LLVMFunction *appendFunc = nullptr;
    static inline LLVMFunction *prependFunc = nullptr;
    static inline LLVMFunction *sliceFunc = nullptr;
    static inline LLVMFunction *equalsFunc = nullptr;
    static inline LLVMFunction *indexOfFunc = nullptr;
    static inline LLVMFunction *trimLeftFunc = nullptr;
    static inline LLVMFunction *trimRightFunc = nullptr;
    static inline LLVMFunction *trimFunc = nullptr;
};

class BuiltinIO {
public:
    static void initialize(LLVMModule &module, LLVMContext &context);

    static inline LLVMFunction *integer2stringFunc = nullptr;
    static inline LLVMFunction *string2integerFunc = nullptr;
    static inline LLVMFunction *printIntegerFunc = nullptr;
    static inline LLVMFunction *printlnIntegerFunc = nullptr;
    static inline LLVMFunction *printStringFunc = nullptr;
    static inline LLVMFunction *printlnStringFunc = nullptr;
};
