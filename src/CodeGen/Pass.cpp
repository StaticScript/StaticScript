#include "CodeGen/Pass.h"

EraseInstsAfterTerminatorPass::EraseInstsAfterTerminatorPass() : FunctionPass(id) {}

bool EraseInstsAfterTerminatorPass::runOnFunction(LLVMFunction &func) {
    Vector<llvm::Instruction *> redundantInsts;
    for (LLVMBasicBlock &bb : func) {
        LLVMBasicBlock::iterator instIt = bb.begin();
        while (!instIt->isTerminator()) {
            ++instIt;
        }
        if (instIt != bb.back().getIterator()) {
            ++instIt;
            while (instIt != bb.end()) {
                redundantInsts.push_back(&(*instIt));
                ++instIt;
            }
        }
    }
    for (auto &inst : redundantInsts) {
        inst->eraseFromParent();
    }
    return true;
}

void runPasses(LLVMModule &llvmModule) {
    llvm::legacy::PassManager passes;
    passes.add(new EraseInstsAfterTerminatorPass());
    passes.run(llvmModule);
}
