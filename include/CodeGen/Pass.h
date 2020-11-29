#pragma once

#include "Support/Alias.h"
#include "Support/LLVM.h"

/// 移除终结指令后面的指令
struct EraseInstsAfterTerminatorPass : public llvm::FunctionPass {
    static inline char id = 0;

    EraseInstsAfterTerminatorPass();

    bool runOnFunction(LLVMFunction &func) override;
};

void runPasses(LLVMModule &llvmModule);
