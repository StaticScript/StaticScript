#pragma once

#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <llvm/ADT/APInt.h>
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
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

using String = std::string;

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using Stack = std::stack<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using SharedPtrVector = Vector<SharedPtr<T>>;

template<typename K, typename V>
using SharedPtrMap = Map<K, SharedPtr<V>>;

template<typename T, typename ...Args>
inline UniquePtr<T> makeUnique(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ...Args>
inline SharedPtr<T> makeShared(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename P>
inline SharedPtr<T> staticPtrCast(const SharedPtr<P> &ptr) {
    return std::static_pointer_cast<T>(ptr);
}

template<typename T, typename P>
inline SharedPtr<T> dynPtrCast(const SharedPtr<P> &ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

using LLVMValue = llvm::Value;
using LLVMContext = llvm::LLVMContext;
using LLVMIRBuilder = llvm::IRBuilder<>;
using LLVMModule = llvm::Module;
using LLVMConstantInt = llvm::ConstantInt;
using LLVMAPInt = llvm::APInt;
using LLVMType = llvm::Type;
using LLVMFunctionType = llvm::FunctionType;
using LLVMFunction = llvm::Function;
using LLVMBasicBlock = llvm::BasicBlock;
using LLVMGlobalValue = llvm::GlobalValue;
using LLVMGlobalVariable = llvm::GlobalVariable;

template<typename X, typename Y>
inline bool LLVMIsa(const Y &val) {
    return llvm::isa<X>(val);
}

template<typename X, typename Y>
inline X* LLVMCast(const Y &val) {
    return llvm::cast<X>(val);
}

template<typename X, typename Y>
inline X* LLVMDynCast(const Y &val) {
    return llvm::dyn_cast<X>(val);
}

inline bool LLVMVerifyFunction(const LLVMFunction &func) {
    return llvm::verifyFunction(func);
}

inline bool LLVMVerifyModule(const LLVMModule &module) {
    return llvm::verifyModule(module);
}
