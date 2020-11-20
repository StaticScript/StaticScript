#pragma once

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "Support/Alias.h"

/// 移除终结指令后面的指令
struct EraseInstsAfterTerminatorPass : public llvm::FunctionPass {
    static inline char id = 0;

    EraseInstsAfterTerminatorPass();

    bool runOnFunction(LLVMFunction &func) override;
};

void runPasses(LLVMModule &llvmModule);
