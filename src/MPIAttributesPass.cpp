#include "MPIAttributesPass.hpp"

#include "Bookkeeper.hpp"
#include "MPIAnnotationFunctions.hpp"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

Bookkeeper bookkeeper;
// Commandline argument to switch on printing an annotation report
static llvm::cl::opt<bool>
    printReport("print-annotation-report",
                llvm::cl::desc("Print a report on the annotated MPI functions"));

/*
   Function that performs the actual work.
   It's separated so it can be called from both the legacy pass and the new pass.
   It works on the principle of testing whether each function exists and then adding attributes
   accordingly
*/
bool runMPIAttributesPass(llvm::Module &M) {
    if (printReport) {
        bookkeeper.annotateModule(M);
    }
    llvm::outs() << "Starting annotating MPI functions...";

    annotateMPISetupTeardown(M);
    annotateMPICommunicationControl(M);
    annotateMPIPointToPointBlocking(M);
    annotateMPIPointToPointNonblocking(M);
    annotateMPIPointToPointPersistent(M);
    annotateMPICollectiveBlocking(M);
    annotateMPICollectiveNonblocking(M);
    annotateMPICollectivePersistent(M);

    llvm::outs() << "Finished annotating MPI functions!\n";
    // BEWARE:  Reports all annotated functions, even those that are removed
    //          during later optimizations. All code included from header files
    //          is also counted. When using e.g. OpenMPI that means the C++
    //          bindings and their use of MPI functions in headers may also be
    //          also reported, even though they are not part of the users's code
    if (printReport) {
        bookkeeper.report();
    }

    // we potentially did modify the module
    return true;
}

void annotateMPISetupTeardown(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Init")) {
        annotateMPIInitFinalize(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Finalize")) {
        annotateMPIInitFinalize(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_size")) {
        annotateMPICommSizeRank(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_rank")) {
        annotateMPICommSizeRank(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_attach_buffer")) {
        annotateMPICommSessionAttachBuffer(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Session_attach_buffer")) {
        annotateMPICommSessionAttachBuffer(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Buffer_attach")) {
        annotateMPIBufferAttach(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Comm_detach_buffer")) {
        annotateMPICommSessionDetachBuffer(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Session_detach_buffer")) {
        annotateMPICommSessionDetachBuffer(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Buffer_detach")) {
        annotateMPIBufferDetach(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPICommunicationControl(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Wait")) {
        annotateMPIWait(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Test")) {
        annotateMPITest(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Waitany")) {
        annotateMPIWaitany(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Testany")) {
        annotateMPITestany(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Waitall")) {
        annotateMPIWaitall(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Testall")) {
        annotateMPITestall(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Waitsome")) {
        annotateMPIWaitTestsome(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Testsome")) {
        annotateMPIWaitTestsome(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Probe")) {
        annotateMPIProbe(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iprobe")) {
        annotateMPIIprobe(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Cancel")) {
        annotateMPICancel(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Test_cancelled")) {
        annotateMPITestCancelled(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Start")) {
        annotateMPIStart(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Startall")) {
        annotateMPIStartall(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPIPointToPointBlocking(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Send")) {
        annotateMPISend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Bsend")) {
        annotateMPISend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Rsend")) {
        annotateMPISend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ssend")) {
        annotateMPISend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Recv")) {
        annotateMPIRecv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Sendrecv")) {
        annotateMPISendrecv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Sendrecv_replace")) {
        annotateMPISendrecvReplace(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPIPointToPointNonblocking(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Isend")) {
        annotateMPIIsend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ibsend")) {
        annotateMPIIsend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Irsend")) {
        annotateMPIIsend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Issend")) {
        annotateMPIIsend(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Irecv")) {
        annotateMPIRecv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Isendrecv")) {
        annotateMPISendrecv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Isendrecv_replace")) {
        annotateMPISendrecvReplace(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPIPointToPointPersistent(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Send_init")) {
        annotateMPISendRecvInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Bsend_init")) {
        annotateMPISendRecvInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Rsend_init")) {
        annotateMPISendRecvInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ssend_init")) {
        annotateMPISendRecvInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Recv_init")) {
        annotateMPISendRecvInit(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPICollectiveBlocking(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Barrier")) {
        annotateMPIBarrier(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Bcast")) {
        annotateMPIBcast(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Gather")) {
        annotateMPIGather(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Gatherv")) {
        annotateMPIGatherv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Scatter")) {
        annotateMPIScatter(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Scatterv")) {
        annotateMPIScatterv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Allgather")) {
        annotateMPIAllgatherAlltoall(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Allgatherv")) {
        annotateMPIAllgatherv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoall")) {
        annotateMPIAllgatherAlltoall(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallv")) {
        annotateMPIAlltoallv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallw")) {
        annotateMPIAlltoallw(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce")) {
        annotateMPIReduceAllreduce(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Allreduce")) {
        annotateMPIReduceAllreduce(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_local")) {
        annotateMPIReduceLocal(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter_block")) {
        annotateMPIReduceScatterBlock(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter")) {
        annotateMPIReduceScatter(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Scan")) {
        annotateMPIScanExscan(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Exscan")) {
        annotateMPIScanExscan(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPICollectiveNonblocking(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Ibarrier")) {
        annotateMPIIbarrier(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ibcast")) {
        annotateMPIIbcast(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Igather")) {
        annotateMPIIgather(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Igatherv")) {
        annotateMPIIgatherv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iscatter")) {
        annotateMPIIscatter(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iscatterv")) {
        annotateMPIIscatterv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iallgather")) {
        annotateMPIIallgatherIalltoall(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iallgatherv")) {
        annotateMPIIallgatherv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ialltoall")) {
        annotateMPIIallgatherIalltoall(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ialltoallv")) {
        annotateMPIIalltoallv(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ialltoallw")) {
        annotateMPIIalltoallw(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ireduce")) {
        annotateMPIIreduce(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iallreduce")) {
        annotateMPIIallreduce(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ireduce_scatter_block")) {
        annotateMPIIreduceScatterBlock(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Ireduce_scatter")) {
        annotateMPIIreduceScatter(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iscan")) {
        annotateMPIIscanIexscan(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Iexscan")) {
        annotateMPIIscanIexscan(func);
        bookkeeper.annotateFunction(func);
    }
}

void annotateMPICollectivePersistent(llvm::Module &M) {
    if (auto *func = M.getFunction("MPI_Barrier_init")) {
        annotateMPIBarrierInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Bcast_init")) {
        annotateMPIBcastInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Gather_init")) {
        annotateMPIGatherScatterInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Gatherv_init")) {
        annotateMPIGathervInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Scatter_init")) {
        annotateMPIGatherScatterInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Scatterv_init")) {
        annotateMPIScattervInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Allgather_init")) {
        annotateMPIAllgatherAlltoallInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Allgatherv_init")) {
        annotateMPIAllgathervInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoall_init")) {
        annotateMPIAllgatherAlltoallInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallv_init")) {
        annotateMPIAlltoallvInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Alltoallw_init")) {
        annotateMPIAlltoallwInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_init")) {
        annotateMPIReduceInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Allreduce_init")) {
        annotateMPIAllreduceInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter_block_init")) {
        annotateMPIReduceScatterBlockInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Reduce_scatter_init")) {
        annotateMPIReduceScatterInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Scan_init")) {
        annotateMPIScanExscanInit(func);
        bookkeeper.annotateFunction(func);
    }
    if (auto *func = M.getFunction("MPI_Exscan_init")) {
        annotateMPIScanExscanInit(func);
        bookkeeper.annotateFunction(func);
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
