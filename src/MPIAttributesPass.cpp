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
    annotateMPISetupTeardown(M);
    annotateMPICommunicationControl(M);
    annotateMPIPointToPointBlocking(M);
    annotateMPIPointToPointNonblocking(M);
    annotateMPIPointToPointPersistent(M);
    annotateMPICollectiveBlocking(M);
    annotateMPICollectiveNonblocking(M);
    annotateMPICollectivePersistent(M);

    // we potentially did modify the module
    return true;
}

void annotateMPISetupTeardown(llvm::Module &M) {
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
}

void annotateMPICommunicationControl(llvm::Module &M) {
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
    if (auto *func = M.getFunction("MPI_Cancel")) {
        annotateMPICancel(func);
    }
    if (auto *func = M.getFunction("MPI_Test_cancelled")) {
        annotateMPITestCancelled(func);
    }
    if (auto *func = M.getFunction("MPI_Start")) {
        annotateMPIStart(func);
    }
    if (auto *func = M.getFunction("MPI_Startall")) {
        annotateMPIStartall(func);
    }
}

void annotateMPIPointToPointBlocking(llvm::Module &M) {
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
}

void annotateMPIPointToPointNonblocking(llvm::Module &M) {
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
}

void annotateMPIPointToPointPersistent(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Send_init")) {
        annotateMPISendRecvInit(func);
    }
    if (auto *func = M.getFunction("MPI_Bsend_init")) {
        annotateMPISendRecvInit(func);
    }
    if (auto *func = M.getFunction("MPI_Rsend_init")) {
        annotateMPISendRecvInit(func);
    }
    if (auto *func = M.getFunction("MPI_Ssend_init")) {
        annotateMPISendRecvInit(func);
    }
    if (auto *func = M.getFunction("MPI_Recv_init")) {
        annotateMPISendRecvInit(func);
    }
}

void annotateMPICollectiveBlocking(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Barrier")) {
        annotateMPIBarrier(func);
    }
    if (auto *func = M.getFunction("MPI_Bcast")) {
        annotateMPIBcast(func);
    }
    if (auto *func = M.getFunction("MPI_Gather")) {
        annotateMPIGather(func);
    }
    if (auto *func = M.getFunction("MPI_Gatherv")) {
        annotateMPIGatherv(func);
    }
    if (auto *func = M.getFunction("MPI_Scatter")) {
        annotateMPIScatter(func);
    }
    if (auto *func = M.getFunction("MPI_Scatterv")) {
        annotateMPIScatterv(func);
    }
    if (auto *func = M.getFunction("MPI_Allgather")) {
        annotateMPIAllgatherAlltoall(func);
    }
    if (auto *func = M.getFunction("MPI_Allgatherv")) {
        annotateMPIAllgatherv(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoall")) {
        annotateMPIAllgatherAlltoall(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallv")) {
        annotateMPIAlltoallv(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallw")) {
        annotateMPIAlltoallw(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce")) {
        annotateMPIReduceAllreduce(func);
    }
    if (auto *func = M.getFunction("MPI_Allreduce")) {
        annotateMPIReduceAllreduce(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_local")) {
        annotateMPIReduceLocal(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter_block")) {
        annotateMPIReduceScatterBlock(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter")) {
        annotateMPIReduceScatter(func);
    }
    if (auto *func = M.getFunction("MPI_Scan")) {
        annotateMPIScanExscan(func);
    }
    if (auto *func = M.getFunction("MPI_Exscan")) {
        annotateMPIScanExscan(func);
    }
}

void annotateMPICollectiveNonblocking(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Ibarrier")) {
        annotateMPIIbarrier(func);
    }
    if (auto *func = M.getFunction("MPI_Ibcast")) {
        annotateMPIIbcast(func);
    }
    if (auto *func = M.getFunction("MPI_Igather")) {
        annotateMPIIgather(func);
    }
    if (auto *func = M.getFunction("MPI_Igatherv")) {
        annotateMPIIgatherv(func);
    }
    if (auto *func = M.getFunction("MPI_Iscatter")) {
        annotateMPIIscatter(func);
    }
    if (auto *func = M.getFunction("MPI_Iscatterv")) {
        annotateMPIIscatterv(func);
    }
    if (auto *func = M.getFunction("MPI_Iallgather")) {
        annotateMPIIallgatherIalltoall(func);
    }
    if (auto *func = M.getFunction("MPI_Iallgatherv")) {
        annotateMPIIallgatherv(func);
    }
    if (auto *func = M.getFunction("MPI_Ialltoall")) {
        annotateMPIIallgatherIalltoall(func);
    }
    if (auto *func = M.getFunction("MPI_Ialltoallv")) {
        annotateMPIIalltoallv(func);
    }
    if (auto *func = M.getFunction("MPI_Ialltoallw")) {
        annotateMPIIalltoallw(func);
    }
    if (auto *func = M.getFunction("MPI_Ireduce")) {
        annotateMPIIreduce(func);
    }
    if (auto *func = M.getFunction("MPI_Iallreduce")) {
        annotateMPIIallreduce(func);
    }
    if (auto *func = M.getFunction("MPI_Ireduce_scatter_block")) {
        annotateMPIIreduceScatterBlock(func);
    }
    if (auto *func = M.getFunction("MPI_Ireduce_scatter")) {
        annotateMPIIreduceScatter(func);
    }
    if (auto *func = M.getFunction("MPI_Iscan")) {
        annotateMPIIscanIexscan(func);
    }
    if (auto *func = M.getFunction("MPI_Iexscan")) {
        annotateMPIIscanIexscan(func);
    }
}

void annotateMPICollectivePersistent(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Barrier_init")) {
        annotateMPIBarrierInit(func);
    }
    if (auto *func = M.getFunction("MPI_Bcast_init")) {
        annotateMPIBcastInit(func);
    }
    if (auto *func = M.getFunction("MPI_Gather_init")) {
        annotateMPIGatherScatterInit(func);
    }
    if (auto *func = M.getFunction("MPI_Gatherv_init")) {
        annotateMPIGathervInit(func);
    }
    if (auto *func = M.getFunction("MPI_Scatter_init")) {
        annotateMPIGatherScatterInit(func);
    }
    if (auto *func = M.getFunction("MPI_Scatterv_init")) {
        annotateMPIScattervInit(func);
    }
    if (auto *func = M.getFunction("MPI_Allgather_init")) {
        annotateMPIAllgatherAlltoallInit(func);
    }
    if (auto *func = M.getFunction("MPI_Allgatherv_init")) {
        annotateMPIAllgathervInit(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoall_init")) {
        annotateMPIAllgatherAlltoallInit(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallv_init")) {
        annotateMPIAlltoallvInit(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallw_init")) {
        annotateMPIAlltoallwInit(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_init")) {
        annotateMPIReduceInit(func);
    }
    if (auto *func = M.getFunction("MPI_Allreduce_init")) {
        annotateMPIAllreduceInit(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter_block_init")) {
        annotateMPIReduceScatterBlockInit(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter_init")) {
        annotateMPIReduceScatterInit(func);
    }
    if (auto *func = M.getFunction("MPI_Scan_init")) {
        annotateMPIScanExscanInit(func);
    }
    if (auto *func = M.getFunction("MPI_Exscan_init")) {
        annotateMPIScanExscanInit(func);
    }
}

/****************************************************************\
|                                                                |
|   from here on boilerplate code to get it all up and running   |
|                                                                |
\****************************************************************/
llvm::PreservedAnalyses MPIAttributesPass::run(llvm::Module &M,
                                               llvm::ModuleAnalysisManager & /*MAM*/) {
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
