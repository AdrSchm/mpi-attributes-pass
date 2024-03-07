#include "MPIAnnotationFunctions.hpp"

// we don't actually need any MPI functions, only the macro that defines MPI_IN_PLACE
#ifdef USE_OPENMPI
#include <mpi.h>
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

    // communicator
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);
}

void annotateMPIBcast(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // data type
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // communicator
    F->addParamAttr(4, ReadOnly);
    F->addParamAttr(4, NoCapture);
#endif
}

void annotateMPIGather(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // send data type
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);
#endif

    // receive buffer
    F->addParamAttr(3, NoCapture);

#ifdef USE_OPENMPI
    // receive data type
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, ReadOnly);

    // communicator
    F->addParamAttr(7, ReadOnly);
    F->addParamAttr(7, NoCapture);
#endif

// As of 3/7/24 this works, though it is possible that it changes in the future
#ifdef USE_OPENMPI
    // get all function calls
    for (auto user : F->users()) {
        // just to make sure
        if (auto *call = llvm::dyn_cast<llvm::CallBase>(user)) {
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
                        }
                    }
                }
                // This call doesn't use MPI_IN_PLACE
            } else {
                call->addParamAttr(0, ReadOnly);
                call->addParamAttr(3, WriteOnly);
            }
        }
    }
#endif
}
