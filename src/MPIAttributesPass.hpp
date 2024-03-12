#ifndef MPI_ATTRIBUTES_PASS_HPP_
#define MPI_ATTRIBUTES_PASS_HPP_

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"

// this is the actual functionality
bool runMPIAttributesPass(llvm::Module &M);
void annotateMPISetupTeardown(llvm::Module &M);
void annotateMPIPointToPointBlocking(llvm::Module &M);
void annotateMPIPointToPointNonblocking(llvm::Module &M);
void annotateMPICollectiveBlocking(llvm::Module &M);
void annotateMPICollectiveNonblocking(llvm::Module &M);

// used for legacy pass manager
struct LegacyMPIAttributesPass : public llvm::ModulePass {
  public:
    static char ID;
    LegacyMPIAttributesPass() : llvm::ModulePass(ID) {}
    bool runOnModule(llvm::Module &M) override;
};

// used for new pass manager
struct MPIAttributesPass : llvm::PassInfoMixin<MPIAttributesPass> {
  public:
    static llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
};

#endif
