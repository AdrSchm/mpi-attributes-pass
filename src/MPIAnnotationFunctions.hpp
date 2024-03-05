#ifndef MPI_ANNOTATION_FUNCTIONS_HPP_
#define MPI_ANNOTATION_FUNCTIONS_HPP_

#include "llvm/IR/Function.h"

// setup and teardown
void annotateMPIInitFinalize(llvm::Function *F);
void annotateMPICommSizeRank(llvm::Function *F);
void annotateMPICommSessionAttachBuffer(llvm::Function *F);
void annotateMPIBufferAttach(llvm::Function *F);
void annotateMPICommSessionDetachBuffer(llvm::Function *F);
void annotateMPIBufferDetach(llvm::Function *F);

// point-to-point communication
void annotateMPISend(llvm::Function *F);
void annotateMPIRecv(llvm::Function *F);
void annotateMPISendrecv(llvm::Function *F);
void annotateMPISendrecvReplace(llvm::Function *F);

// collective communication

#endif