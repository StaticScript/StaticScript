#pragma once

#include "Support/Alias.h"
#include "Support/LLVM.h"
#include "Support/Error.h"

class Optimizer final {
public:
    explicit Optimizer(LLVMModule &module, unsigned optLevel, unsigned sizeLevel);

    void optimize();

private:
    void preoptimize();

    void addOptimizationPasses(llvm::legacy::PassManager &passManager,
                               llvm::legacy::FunctionPassManager &functionPassManager,
                               llvm::TargetMachine *targetMachine) const;

    static void addStandardLinkPasses(llvm::legacy::PassManagerBase &passManager);

    LLVMModule &module;
    unsigned optLevel;
    unsigned sizeLevel;
};