#include "MPIAnnotationFunctions.hpp"

// we don't actually need any MPI functions, only the macro that defines MPI_IN_PLACE
#ifdef USE_OPENMPI
#include <mpi.h>
#ifndef OPEN_MPI
#error "Building for OpenMPI without OpenMPI header"
#endif
#endif

#include "llvm/IR/Argument.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"

// Use shorter names for needed attributes
const auto ReadOnly = llvm::Attribute::AttrKind::ReadOnly;
const auto WriteOnly = llvm::Attribute::AttrKind::WriteOnly;
const auto ReadNone = llvm::Attribute::AttrKind::ReadNone;
const auto NoCapture = llvm::Attribute::AttrKind::NoCapture;

void annotateMPIBarrier(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();
}

void annotateMPIBcast(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, NoCapture);
}

void annotateMPIGather(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // Potentially the send buffer is specified as MPI_IN_PLACE, which would make it ReadNone.
    // However, as this probably wouldn't lead to any optimizations, we take the simpler ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // We could mark this as ReadNone if this is not called in the root process, but this requires
    // runtime information.
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);
}

void annotateMPIGatherv(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // Potentially the send buffer is specified as MPI_IN_PLACE, which would make it ReadNone.
    // However, as this probably wouldn't lead to any optimizations, we take the simpler ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // We could mark this as ReadNone if this is not called in the root process, but this requires
    // runtime information.
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);

    // receive count array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    F->addParamAttr(4, ReadOnly);
    F->addParamAttr(4, NoCapture);

    // displacements array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);
}

void annotateMPIScatter(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // Potentially this gets specified as MPI_IN_PLACE. In this case we could mark this as ReadNone,
    // though as there probably aren't any optimizations that can be done in this case, we take the
    // simpler WriteOnly.
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);
}

void annotateMPIScatterv(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // send count array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    F->addParamAttr(1, ReadOnly);
    F->addParamAttr(1, NoCapture);

    // displacements array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // receive buffer
    // Potentially this gets specified as MPI_IN_PLACE. In this case we could mark this as ReadNone,
    // though as there probably aren't any optimizations that can be done in this case, we take the
    // simpler WriteOnly.
    F->addParamAttr(4, WriteOnly);
    F->addParamAttr(4, NoCapture);
}

void annotateMPIAllgatherAlltoall(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // This could be specified as MPI_IN_PLACE, which would allow us to mark it as ReadNone.
    // However, this again probably wouldn't result in any optimizations, so we take ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(3, NoCapture);
}

void annotateMPIAllgatherv(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // This could be specified as MPI_IN_PLACE, which would allow us to mark it as ReadNone.
    // However, this again probably wouldn't result in any optimizations, so we take ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(3, NoCapture);

    // receive count array
    F->addParamAttr(4, ReadOnly);
    F->addParamAttr(4, NoCapture);

    // displacements array
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);
}

void annotateMPIAlltoallv(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, NoCapture);

    // send count array
    F->addParamAttr(1, NoCapture);

    // send displacements array
    F->addParamAttr(2, NoCapture);

// As of 3/7/24 this works, though it is possible that it changes in the future
#ifdef USE_OPENMPI
    // get all function calls
    for (auto user : F->users()) {
        // just to make sure
        if (auto *call = llvm::dyn_cast<llvm::CallBase>(user)) {
            bool failed = false;
            // get send buffer argument
            auto sendBuf = call->arg_begin()->get();
            // make sure that we have MPI_IN_PLACE
            if (auto constExpr = llvm::dyn_cast<llvm::ConstantExpr>(sendBuf);
                constExpr && constExpr->isCast()) {
                if (auto asInstr =
                        llvm::dyn_cast<llvm::IntToPtrInst>(constExpr->getAsInstruction())) {
                    auto val = asInstr->getOperand(0);
                    if (auto intConst = llvm::dyn_cast<llvm::ConstantInt>(val)) {
                        if (intConst->getValue() == (uint64_t)MPI_IN_PLACE) {
                            call->addParamAttr(0, ReadNone);
                            call->addParamAttr(1, ReadNone);
                            call->addParamAttr(2, ReadNone);
                        } else {
                            // Some other constant than MPI_IN_PLACE
                            failed = true;
                        }
                    } else {
                        // Cast is not done on a constant integer, but e.g. on a variable
                        failed = true;
                    }
                } else {
                    // Constant expression is not a ptr-to-int cast
                    failed = true;
                }
            } else {
                // It is not a constant expression
                failed = true;
            }
            // This call doesn't use MPI_IN_PLACE
            if (failed) {
                call->addParamAttr(0, ReadOnly);
                call->addParamAttr(1, ReadOnly);
                call->addParamAttr(2, ReadOnly);
            }
        }
    }
#else
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(1, ReadOnly);
    F->addParamAttr(2, ReadOnly);
#endif

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(4, NoCapture);

    // receive count array
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);

    // receive displacements array
    F->addParamAttr(6, ReadOnly);
    F->addParamAttr(6, NoCapture);
}

void annotateMPIAlltoallw(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, NoCapture);

    // send count array
    F->addParamAttr(1, NoCapture);

    // send displacements array
    F->addParamAttr(2, NoCapture);

    // send data types array
    F->addParamAttr(3, NoCapture);

// As of 3/7/24 this works, though it is possible that it changes in the future
#ifdef USE_OPENMPI
    // get all function calls
    for (auto user : F->users()) {
        // just to make sure
        if (auto *call = llvm::dyn_cast<llvm::CallBase>(user)) {
            bool failed = false;
            // get send buffer argument
            auto sendBuf = call->arg_begin()->get();
            // make sure that we have MPI_IN_PLACE
            if (auto constExpr = llvm::dyn_cast<llvm::ConstantExpr>(sendBuf);
                constExpr && constExpr->isCast()) {
                if (auto asInstr =
                        llvm::dyn_cast<llvm::IntToPtrInst>(constExpr->getAsInstruction())) {
                    auto val = asInstr->getOperand(0);
                    if (auto intConst = llvm::dyn_cast<llvm::ConstantInt>(val)) {
                        if (intConst->getValue() == (uint64_t)MPI_IN_PLACE) {
                            call->addParamAttr(0, ReadNone);
                            call->addParamAttr(1, ReadNone);
                            call->addParamAttr(2, ReadNone);
                            call->addParamAttr(3, ReadNone);
                        } else {
                            // Some other constant than MPI_IN_PLACE
                            failed = true;
                        }
                    } else {
                        // Cast is not done on a constant integer, but e.g. on a variable
                        failed = true;
                    }
                } else {
                    // Constant expression is not a ptr-to-int cast
                    failed = true;
                }
            } else {
                // It is not a constant expression
                failed = true;
            }
            // This call doesn't use MPI_IN_PLACE
            if (failed) {
                call->addParamAttr(0, ReadOnly);
                call->addParamAttr(1, ReadOnly);
                call->addParamAttr(2, ReadOnly);
                call->addParamAttr(3, ReadOnly);
            }
        }
    }
#else
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(1, ReadOnly);
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(3, ReadOnly);
#endif

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(4, NoCapture);

    // receive count array
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);

    // receive displacements array
    F->addParamAttr(6, ReadOnly);
    F->addParamAttr(6, NoCapture);

    // receive data types array
    F->addParamAttr(7, ReadOnly);
    F->addParamAttr(7, NoCapture);
}

void annotateMPIReduce(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // This could be specified as MPI_IN_PLACE, which would allow us to mark it as ReadNone.
    // However, this again probably wouldn't result in any optimizations, so we take ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(1, NoCapture);
}

void annotateMPIAllreduce(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // This could be specified as MPI_IN_PLACE, which would allow us to mark it as ReadNone.
    // However, this again probably wouldn't result in any optimizations, so we take ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(1, NoCapture);
}

void annotateMPIReduceLocal(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // input buffer
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // input output buffer
    F->addParamAttr(1, NoCapture);
}

void annotateMPIReduceScatterBlock(llvm::Function *F) {
    // same behavior, this is only separate for better readability
    annotateMPIAllreduce(F);
}

void annotateMPIReduceScatter(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    // This could be specified as MPI_IN_PLACE, which would allow us to mark it as ReadNone.
    // However, this again probably wouldn't result in any optimizations, so we take ReadOnly.
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // receive buffer
    // If we could be sure that the call is not happening "in place" we could mark this as
    // WriteOnly, but this requires runtime information.
    F->addParamAttr(1, NoCapture);

    // receive count array
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);
}

void annotateMPIScanExscan(llvm::Function *F) {
    // same behavior, this is only separate for better readability
    // only difference in Exscan is that receive buffer of process with rank 0 could be marked
    // ReadNone if it is not "in place", but this again would require runtime information
    annotateMPIAllreduce(F);
}