#pragma once

#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/InitializePasses.h>
#include <llvm/PassRegistry.h>
#include "Support/Exception.h"

inline void initLLVMTarget() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
}

inline void initLLVMPasses() {
    llvm::PassRegistry &registry = *llvm::PassRegistry::getPassRegistry();
    llvm::initializeCore(registry);
    llvm::initializeCoroutines(registry);
    llvm::initializeScalarOpts(registry);
    llvm::initializeObjCARCOpts(registry);
    llvm::initializeVectorization(registry);
    llvm::initializeIPO(registry);
    llvm::initializeAnalysis(registry);
    llvm::initializeTransformUtils(registry);
    llvm::initializeInstCombine(registry);
    llvm::initializeAggressiveInstCombine(registry);
    llvm::initializeInstrumentation(registry);
    llvm::initializeTarget(registry);
    llvm::initializeExpandMemCmpPassPass(registry);
    llvm::initializeScalarizeMaskedMemIntrinPass(registry);
    llvm::initializeCodeGenPreparePass(registry);
    llvm::initializeAtomicExpandPass(registry);
    llvm::initializeRewriteSymbolsLegacyPassPass(registry);
    llvm::initializeWinEHPreparePass(registry);
    llvm::initializeDwarfEHPreparePass(registry);
    llvm::initializeSafeStackLegacyPassPass(registry);
    llvm::initializeSjLjEHPreparePass(registry);
    llvm::initializePreISelIntrinsicLoweringLegacyPassPass(registry);
    llvm::initializeGlobalMergePass(registry);
    llvm::initializeIndirectBrExpandPassPass(registry);
    llvm::initializeInterleavedLoadCombinePass(registry);
    llvm::initializeInterleavedAccessPass(registry);
    llvm::initializeEntryExitInstrumenterPass(registry);
    llvm::initializePostInlineEntryExitInstrumenterPass(registry);
    llvm::initializeUnreachableBlockElimLegacyPassPass(registry);
    llvm::initializeExpandReductionsPass(registry);
    llvm::initializeWasmEHPreparePass(registry);
    llvm::initializeWriteBitcodePassPass(registry);
    llvm::initializeHardwareLoopsPass(registry);
    llvm::initializeTypePromotionPass(registry);
}

inline void initLLVMCodeGen() {
    static llvm::codegen::RegisterCodeGenFlags CFG;
}

inline llvm::CodeGenOpt::Level getCodeGenOptLevel(unsigned optLevel = 2) {
    switch (optLevel) {
        case 1:
            return llvm::CodeGenOpt::Less;
        case 2:
            return llvm::CodeGenOpt::Default;
        case 3:
            return llvm::CodeGenOpt::Aggressive;
        default:
            return llvm::CodeGenOpt::None;
    }
}

inline String getTargetTriple() {
    return llvm::sys::getDefaultTargetTriple();
}

inline llvm::TargetMachine *getTargetMachine(unsigned optLevel = 2) {
    const String &targetTriple = getTargetTriple();
    String error;
    const llvm::Target *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) {
        throw DriverException(error);
    }
    return target->createTargetMachine(targetTriple,
                                       llvm::codegen::getCPUStr(),
                                       llvm::codegen::getFeaturesStr(),
                                       llvm::codegen::InitTargetOptionsFromCodeGenFlags(),
                                       llvm::codegen::getExplicitRelocModel(),
                                       llvm::codegen::getExplicitCodeModel(),
                                       getCodeGenOptLevel(optLevel));
}