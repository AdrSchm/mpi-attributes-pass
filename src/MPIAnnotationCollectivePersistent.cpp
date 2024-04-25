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

void annotateMPIBarrierInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);
}

void annotateMPIBcastInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, ReadNone);

    // request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
}

void annotateMPIGatherScatterInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(3, ReadNone);

    // request
    F->addParamAttr(9, WriteOnly);
    F->addParamAttr(9, NoCapture);
}

void annotateMPIGathervInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(3, ReadNone);

    // receive count array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(4, ReadOnly);
    F->addParamAttr(4, NoCapture);

    // displacements array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);

    // request
    F->addParamAttr(10, WriteOnly);
    F->addParamAttr(10, NoCapture);
}

void annotateMPIScattervInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // send count array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(1, ReadOnly);
    F->addParamAttr(1, NoCapture);

    // displacements array
    // We could mark this as ReadNone if we could be sure that the process is not the root process
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // receive buffer
    F->addParamAttr(4, ReadNone);

    // request
    F->addParamAttr(10, WriteOnly);
    F->addParamAttr(10, NoCapture);
}

void annotateMPIAllgatherAlltoallInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(3, ReadNone);

    // request
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
}

void annotateMPIAllgathervInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(3, ReadNone);

    // receive count array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(4, ReadOnly);
    F->addParamAttr(4, NoCapture);

    // displacements array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);

    // request
    F->addParamAttr(9, WriteOnly);
    F->addParamAttr(9, NoCapture);
}

void annotateMPIAlltoallvInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // send count array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(1, NoCapture);

    // send displacements array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
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
                call->addParamAttr(1, ReadOnly);
                call->addParamAttr(2, ReadOnly);
            }
        }
    }
#else
    F->addParamAttr(1, ReadOnly);
    F->addParamAttr(2, ReadOnly);
#endif

    // receive buffer
    F->addParamAttr(4, ReadNone);

    // receive count array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);

    // receive displacements array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(6, ReadOnly);
    F->addParamAttr(6, NoCapture);

    // request
    F->addParamAttr(10, WriteOnly);
    F->addParamAttr(10, NoCapture);
}

void annotateMPIAlltoallwInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // send count array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(1, NoCapture);

    // send displacements array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(2, NoCapture);

    // send data types array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
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
                call->addParamAttr(1, ReadOnly);
                call->addParamAttr(2, ReadOnly);
                call->addParamAttr(3, ReadOnly);
            }
        }
    }
#else
    F->addParamAttr(1, ReadOnly);
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(3, ReadOnly);
#endif

    // receive buffer
    F->addParamAttr(4, ReadNone);

    // receive count array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);

    // receive displacements array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(6, ReadOnly);
    F->addParamAttr(6, NoCapture);

    // receive data types array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(7, ReadOnly);
    F->addParamAttr(7, NoCapture);

    // request
    F->addParamAttr(10, WriteOnly);
    F->addParamAttr(10, NoCapture);
}

void annotateMPIReduceInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(1, ReadNone);

    // request
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
}

void annotateMPIAllreduceInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(1, ReadNone);

    // request
    F->addParamAttr(7, WriteOnly);
    F->addParamAttr(7, NoCapture);
}

void annotateMPIReduceScatterBlockInit(llvm::Function *F) {
    // same behavior, this is only separate for better readability
    annotateMPIAllreduceInit(F);
}

void annotateMPIReduceScatterInit(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadNone);

    // receive buffer
    F->addParamAttr(1, ReadNone);

    // receive count array
    // This does capture the pointer, but according to the MPI standard it is erroneous to modify it
    // after initialization, so it is effectively NoCapture
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // request
    F->addParamAttr(7, WriteOnly);
    F->addParamAttr(7, NoCapture);
}

void annotateMPIScanExscanInit(llvm::Function *F) {
    // same behavior, this is only separate for better readability
    annotateMPIAllreduceInit(F);
}
