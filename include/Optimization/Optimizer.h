#pragma once

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include "Support/Alias.h"
#include "Support/LLVM.h"
#include "Support/Exception.h"

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