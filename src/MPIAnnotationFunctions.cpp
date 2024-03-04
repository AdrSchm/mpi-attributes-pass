#include "MPIAnnotationFunctions.hpp"

#include "llvm/IR/Argument.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"

// Use shorter names for needed attributes
const auto ReadOnly = llvm::Attribute::AttrKind::ReadOnly;
const auto WriteOnly = llvm::Attribute::AttrKind::WriteOnly;
const auto NoCapture = llvm::Attribute::AttrKind::NoCapture;

void annotateMPIInitFinalize(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();
}

void annotateMPICommSizeRank(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

#ifdef USE_OPENMPI
    // communicator
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);
#endif

    // buffer
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);
    llvm::Argument *bufferArg = F->arg_begin() + 1;
    llvm::AttrBuilder &sretAttr = llvm::AttrBuilder(F->getContext())
                                      .addStructRetAttr(llvm::Type::getInt32Ty(F->getContext()));
    bufferArg->addAttrs(sretAttr);
}

void annotateMPISend(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // data type
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // communicator
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);
#endif
}

void annotateMPIRecv(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, WriteOnly);
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // data type
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // communicator
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);
#endif

    // status
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
    F->addDereferenceableOrNullParamAttr(6, 12);
}