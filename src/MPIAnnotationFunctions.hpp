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

// Point-to-point communication
// blocking
void annotateMPISend(llvm::Function *F);
void annotateMPIRecv(llvm::Function *F);
void annotateMPISendrecv(llvm::Function *F);
void annotateMPISendrecvReplace(llvm::Function *F);
// nonblocking
void annotateMPIIsend(llvm::Function *F);
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

// Collective communication
void annotateMPIBarrier(llvm::Function *F);
void annotateMPIBcast(llvm::Function *F);
void annotateMPIGather(llvm::Function *F);

#endif