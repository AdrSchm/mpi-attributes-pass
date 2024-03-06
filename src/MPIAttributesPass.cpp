#include "MPIAttributesPass.hpp"

#include "MPIAnnotationFunctions.hpp"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

/*
   Function that performs the actual work.
   It's separated so it can be called from both the legacy pass and the new pass.
   It works on the principle of testing whether each function exists and then adding attributes
   accordingly
*/
bool runMPIAttributesPass(llvm::Module &M) {
    // Setup and teardown operations
    if (auto *func = M.getFunction("MPI_Init")) {
        annotateMPIInitFinalize(func);
    }
    if (auto *func = M.getFunction("MPI_Finalize")) {
        annotateMPIInitFinalize(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_size")) {
        annotateMPICommSizeRank(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_rank")) {
        annotateMPICommSizeRank(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_attach_buffer")) {
        annotateMPICommSessionAttachBuffer(func);
    }
    if (auto *func = M.getFunction("MPI_Session_attach_buffer")) {
        annotateMPICommSessionAttachBuffer(func);
    }
    if (auto *func = M.getFunction("MPI_Buffer_attach")) {
        annotateMPIBufferAttach(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_detach_buffer")) {
        annotateMPICommSessionDetachBuffer(func);
    }
    if (auto *func = M.getFunction("MPI_Session_detach_buffer")) {
        annotateMPICommSessionDetachBuffer(func);
    }
    if (auto *func = M.getFunction("MPI_Buffer_detach")) {
        annotateMPIBufferDetach(func);
    }

    // Point-to-point communication
    // blocking
    if (auto *func = M.getFunction("MPI_Send")) {
        annotateMPISend(func);
    }
    if (auto *func = M.getFunction("MPI_Bsend")) {
        annotateMPISend(func);
    }
    if (auto *func = M.getFunction("MPI_Rsend")) {
        annotateMPISend(func);
    }
    if (auto *func = M.getFunction("MPI_Ssend")) {
        annotateMPISend(func);
    }
    if (auto *func = M.getFunction("MPI_Recv")) {
        annotateMPIRecv(func);
    }
    if (auto *func = M.getFunction("MPI_Sendrecv")) {
        annotateMPISendrecv(func);
    }
    if (auto *func = M.getFunction("MPI_Sendrecv_replace")) {
        annotateMPISendrecvReplace(func);
    }
    // nonblocking
    if (auto *func = M.getFunction("MPI_Isend")) {
        annotateMPIIsend(func);
    }
    if (auto *func = M.getFunction("MPI_Ibsend")) {
        annotateMPIIsend(func);
    }
    if (auto *func = M.getFunction("MPI_Irsend")) {
        annotateMPIIsend(func);
    }
    if (auto *func = M.getFunction("MPI_Issend")) {
        annotateMPIIsend(func);
    }
    if (auto *func = M.getFunction("MPI_Irecv")) {
        annotateMPIRecv(func);
    }
    if (auto *func = M.getFunction("MPI_Isendrecv")) {
        annotateMPISendrecv(func);
    }
    if (auto *func = M.getFunction("MPI_Isendrecv_replace")) {
        annotateMPISendrecvReplace(func);
    }
    if (auto *func = M.getFunction("MPI_Wait")) {
        annotateMPIWait(func);
    }
    if (auto *func = M.getFunction("MPI_Test")) {
        annotateMPITest(func);
    }
    if (auto *func = M.getFunction("MPI_Waitany")) {
        annotateMPIWaitany(func);
    }
    if (auto *func = M.getFunction("MPI_Testany")) {
        annotateMPITestany(func);
    }
    if (auto *func = M.getFunction("MPI_Waitall")) {
        annotateMPIWaitall(func);
    }
    if (auto *func = M.getFunction("MPI_Testall")) {
        annotateMPITestall(func);
    }
    if (auto *func = M.getFunction("MPI_Waitsome")) {
        annotateMPIWaitTestsome(func);
    }
    if (auto *func = M.getFunction("MPI_Testsome")) {
        annotateMPIWaitTestsome(func);
    }
    if (auto *func = M.getFunction("MPI_Probe")) {
        annotateMPIProbe(func);
    }
    if (auto *func = M.getFunction("MPI_Iprobe")) {
        annotateMPIIprobe(func);
    }

    // Collective Communication
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