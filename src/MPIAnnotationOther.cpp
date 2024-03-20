#include "MPIAnnotationFunctions.hpp"

#include "llvm/IR/Argument.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"

// Use shorter names for needed attributes
const auto ReadOnly = llvm::Attribute::AttrKind::ReadOnly;
const auto WriteOnly = llvm::Attribute::AttrKind::WriteOnly;
const auto ReadNone = llvm::Attribute::AttrKind::ReadNone;
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
}

void annotateMPICommSessionAttachBuffer(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->doesNotFreeMemory();

#ifdef USE_OPENMPI
    // communicator or session
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);
#endif

    // buffer
    F->addParamAttr(1, ReadNone);
}

void annotateMPIBufferAttach(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemory();
    F->doesNotFreeMemory();
}

void annotateMPICommSessionDetachBuffer(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->doesNotFreeMemory();

#ifdef USE_OPENMPI
    // communicator or session
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);
#endif

    // buffer address
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);

    // buffer size
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);
}

void annotateMPIBufferDetach(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setOnlyWritesMemory();
    F->setDoesNotFreeMemory();

    F->addParamAttr(0, NoCapture);
    F->addParamAttr(1, NoCapture);
}