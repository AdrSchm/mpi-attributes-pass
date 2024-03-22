#ifndef MPI_ANNOTATION_FUNCTIONS_HPP_
#define MPI_ANNOTATION_FUNCTIONS_HPP_

#include "llvm/IR/Function.h"

// Setup and teardown operations
void annotateMPIInitFinalize(llvm::Function *F);
void annotateMPICommSizeRank(llvm::Function *F);
void annotateMPICommSessionAttachBuffer(llvm::Function *F);
void annotateMPIBufferAttach(llvm::Function *F);
void annotateMPICommSessionDetachBuffer(llvm::Function *F);
void annotateMPIBufferDetach(llvm::Function *F);

// Communication control
void annotateMPIWait(llvm::Function *F);
void annotateMPITest(llvm::Function *F);
void annotateMPIWaitany(llvm::Function *F);
void annotateMPITestany(llvm::Function *F);
void annotateMPIWaitall(llvm::Function *F);
void annotateMPITestall(llvm::Function *F);
void annotateMPIWaitTestsome(llvm::Function *F);
void annotateMPIProbe(llvm::Function *F);
void annotateMPIIprobe(llvm::Function *F);
void annotateMPICancel(llvm::Function *F);
void annotateMPITestCancelled(llvm::Function *F);
void annotateMPIStart(llvm::Function *F);
void annotateMPIStartall(llvm::Function *F);

// Point-to-point communication
// blocking
void annotateMPISend(llvm::Function *F);
void annotateMPIRecv(llvm::Function *F);
void annotateMPISendrecv(llvm::Function *F);
void annotateMPISendrecvReplace(llvm::Function *F);
// nonblocking
void annotateMPIIsend(llvm::Function *F);
// persistent
void annotateMPISendInit(llvm::Function *F);
void annotateMPIRecvInit(llvm::Function *F);

// Collective communication
// blocking
void annotateMPIBarrier(llvm::Function *F);
void annotateMPIBcast(llvm::Function *F);
void annotateMPIGather(llvm::Function *F);
void annotateMPIGatherv(llvm::Function *F);
void annotateMPIScatter(llvm::Function *F);
void annotateMPIScatterv(llvm::Function *F);
void annotateMPIAllgatherAlltoall(llvm::Function *F);
void annotateMPIAllgatherv(llvm::Function *F);
void annotateMPIAlltoallv(llvm::Function *F);
void annotateMPIAlltoallw(llvm::Function *F);
void annotateMPIReduce(llvm::Function *F);
void annotateMPIAllreduce(llvm::Function *F);
void annotateMPIReduceLocal(llvm::Function *F);
void annotateMPIReduceScatterBlock(llvm::Function *F);
void annotateMPIReduceScatter(llvm::Function *F);
void annotateMPIScanExscan(llvm::Function *F);
// nonblocking
void annotateMPIIbarrier(llvm::Function *F);
void annotateMPIIbcast(llvm::Function *F);
void annotateMPIIgather(llvm::Function *F);
void annotateMPIIgatherv(llvm::Function *F);
void annotateMPIIscatter(llvm::Function *F);
void annotateMPIIscatterv(llvm::Function *F);
void annotateMPIIallgatherIalltoall(llvm::Function *F);
void annotateMPIIallgatherv(llvm::Function *F);
void annotateMPIIalltoallv(llvm::Function *F);
void annotateMPIIalltoallw(llvm::Function *F);
void annotateMPIIreduce(llvm::Function *F);
void annotateMPIIallreduce(llvm::Function *F);
void annotateMPIIreduceScatterBlock(llvm::Function *F);
void annotateMPIIreduceScatter(llvm::Function *F);
void annotateMPIIscanIexscan(llvm::Function *F);
// persistent
void annotateMPIBarrierInit(llvm::Function *F);
void annotateMPIBcastInit(llvm::Function *F);
void annotateMPIGatherInit(llvm::Function *F);
void annotateMPIGathervInit(llvm::Function *F);
void annotateMPIScatterInit(llvm::Function *F);
void annotateMPIScattervInit(llvm::Function *F);
void annotateMPIAllgatherAlltoallInit(llvm::Function *F);
void annotateMPIAllgathervInit(llvm::Function *F);
void annotateMPIAlltoallvInit(llvm::Function *F);
void annotateMPIAlltoallwInit(llvm::Function *F);
void annotateMPIReduceInit(llvm::Function *F);
void annotateMPIAllreduceInit(llvm::Function *F);
void annotateMPIReduceScatterBlockInit(llvm::Function *F);
void annotateMPIReduceScatterInit(llvm::Function *F);
void annotateMPIScanExscanInit(llvm::Function *F);

#endif