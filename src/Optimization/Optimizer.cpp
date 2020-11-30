#include "Optimization/Optimizer.h"

Optimizer::Optimizer(LLVMModule &module, unsigned int optLevel, unsigned int sizeLevel) : module(module),
                                                                                          optLevel(optLevel),
                                                                                          sizeLevel(sizeLevel) {}

void Optimizer::optimize() {
    preoptimize();

    llvm::Triple moduleTriple(getTargetTriple());

    llvm::TargetLibraryInfoImpl tlii(moduleTriple);

    llvm::codegen::setFunctionAttributes(llvm::codegen::getCPUStr(), llvm::codegen::getFeaturesStr(), module);

    llvm::legacy::PassManager passes;

    llvm::TargetMachine *targetMachine = getTargetMachine(optLevel);

    passes.add(new llvm::TargetLibraryInfoWrapperPass(tlii));

    passes.add(llvm::createTargetTransformInfoWrapperPass(targetMachine->getTargetIRAnalysis()));

    llvm::legacy::FunctionPassManager funcPasses(&module);

    funcPasses.add(llvm::createTargetTransformInfoWrapperPass(targetMachine->getTargetIRAnalysis()));

    addStandardLinkPasses(passes);

    addOptimizationPasses(passes, funcPasses, targetMachine);

    funcPasses.doInitialization();
    for (LLVMFunction &function : module) {
        funcPasses.run(function);
    }
    funcPasses.doFinalization();

    passes.add(llvm::createVerifierPass());

    passes.run(module);
}

void Optimizer::preoptimize() {
    for (LLVMFunction &function : module) {
        if (function.getName() != "main" && !function.isDeclaration()) {
            function.setLinkage(LLVMFunction::LinkageTypes::InternalLinkage);
        }
    }
}

void Optimizer::addOptimizationPasses(llvm::legacy::PassManager &passManager, llvm::legacy::FunctionPassManager &functionPassManager,
                                      llvm::TargetMachine *targetMachine) const {
    llvm::PassManagerBuilder builder;
    builder.OptLevel = optLevel;
    builder.SizeLevel = sizeLevel;
    if (optLevel > 1) {
        builder.Inliner = llvm::createFunctionInliningPass(optLevel, sizeLevel, false);
    } else {
        builder.Inliner = llvm::createAlwaysInlinerLegacyPass();
    }
    builder.DisableUnrollLoops = optLevel == 0;
    builder.LoopVectorize = optLevel > 1 && sizeLevel < 2;
    builder.SLPVectorize = optLevel > 1 && sizeLevel < 2;
    targetMachine->adjustPassManager(builder);
    builder.populateFunctionPassManager(functionPassManager);
    builder.populateModulePassManager(passManager);
}

void Optimizer::addStandardLinkPasses(llvm::PassManagerBase &passManager) {
    llvm::PassManagerBuilder Builder;
    Builder.VerifyInput = true;
    Builder.Inliner = llvm::createFunctionInliningPass();
    Builder.populateLTOPassManager(passManager);
}