#pragma once

#include <llvm/ADT/Twine.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/InitializePasses.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Pass.h>
#include <llvm/PassRegistry.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include "Support/Error.h"

using LLVMValue = llvm::Value;
using LLVMContext = llvm::LLVMContext;
using LLVMModule = llvm::Module;
using LLVMConstantInt = llvm::ConstantInt;
using LLVMType = llvm::Type;
using LLVMFunctionType = llvm::FunctionType;
using LLVMFunction = llvm::Function;
using LLVMBasicBlock = llvm::BasicBlock;
using LLVMGlobalValue = llvm::GlobalValue;
using LLVMGlobalVariable = llvm::GlobalVariable;

void reportOnDriverError(bool condition, const llvm::Twine &reason);

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
    reportOnDriverError(!target, error);
    return target->createTargetMachine(targetTriple,
                                       llvm::codegen::getCPUStr(),
                                       llvm::codegen::getFeaturesStr(),
                                       llvm::codegen::InitTargetOptionsFromCodeGenFlags(),
                                       llvm::codegen::getExplicitRelocModel(),
                                       llvm::codegen::getExplicitCodeModel(),
                                       getCodeGenOptLevel(optLevel));
}