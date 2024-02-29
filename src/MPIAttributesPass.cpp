#include "MPIAttributesPass.hpp"

#include "llvm/IR/Attributes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <string>

/*
    Function that performs the actual work.
    It's separated so it can be called from both the legacy pass and the new pass.
*/
bool runMPIAttributesPass(llvm::Module &M) {
    // Use shorter names for needed attributes
    const auto ReadOnly = llvm::Attribute::AttrKind::ReadOnly;
    const auto WriteOnly = llvm::Attribute::AttrKind::WriteOnly;
    const auto NoCapture = llvm::Attribute::AttrKind::NoCapture;

    // Test if we have MPI functions and add attributes accordingly
    if (auto *func = M.getFunction("MPI_Send")) {
        func->setOnlyAccessesInaccessibleMemOrArgMem();
        func->setDoesNotFreeMemory();

        // buffer
        func->addParamAttr(0, ReadOnly);
        func->addParamAttr(0, NoCapture);

        // data type
        func->addParamAttr(2, ReadOnly);
        func->addParamAttr(2, NoCapture);

        // communicator
        func->addParamAttr(5, ReadOnly);
        func->addParamAttr(5, NoCapture);
    }
    if (auto *func = M.getFunction("MPI_Recv")) {
        func->setOnlyAccessesInaccessibleMemOrArgMem();

        // buffer
        func->addParamAttr(0, WriteOnly);
        func->addParamAttr(0, NoCapture);

        // data type
        func->addParamAttr(2, ReadOnly);
        func->addParamAttr(2, NoCapture);

        // communicator
        func->addParamAttr(5, ReadOnly);
        func->addParamAttr(5, NoCapture);

        // status
        func->addParamAttr(6, WriteOnly);
        func->addParamAttr(6, NoCapture);
    }
    // we potentially did modify the module
    return true;
}

/****************************************************************\
|                                                                |
|   from here on boilerplate code to get it all up and running   |
|                                                                |
\****************************************************************/
llvm::PreservedAnalyses MPIAttributesPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
    if (!runMPIAttributesPass(M)) {
        return llvm::PreservedAnalyses::all();
    }
    return llvm::PreservedAnalyses::none();
}

bool LegacyMPIAttributesPass::runOnModule(llvm::Module &M) {
    return runMPIAttributesPass(M);
}

// register pass for legacy pass manager
char LegacyMPIAttributesPass::ID = 0;
static llvm::RegisterPass<LegacyMPIAttributesPass>
    X("mpiattrs", "Adds attributes for MPI functions concerning pointer accesses", false, false);

// register pass for new pass manager
llvm::PassPluginLibraryInfo getMPIAttributesPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "MPIAttributesPass", LLVM_VERSION_STRING,
            [](llvm::PassBuilder &PB) {
                PB.registerPipelineEarlySimplificationEPCallback(
                    [&](llvm::ModulePassManager &MPM, auto) {
                        MPM.addPass(MPIAttributesPass());
                        return true;
                    });
                PB.registerPipelineParsingCallback(
                    [](llvm::StringRef Name, llvm::ModulePassManager &PM,
                       llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                        if (Name == "mpiattrs") {
                            PM.addPass(MPIAttributesPass());
                            return true;
                        }
                        return false;
                    });
            }};
}

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getMPIAttributesPassPluginInfo();
}