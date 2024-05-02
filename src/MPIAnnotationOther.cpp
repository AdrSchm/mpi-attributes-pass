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

    // buffer
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);
}

void annotateMPICommSessionAttachBuffer(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->doesNotFreeMemory();

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

void annotateMPIWait(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request
    F->addParamAttr(0, NoCapture);

    // status
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);
}

void annotateMPITest(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request
    F->addParamAttr(0, NoCapture);

    // flag
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);

    // status
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);
}

void annotateMPIWaitany(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request array
    F->addParamAttr(1, NoCapture);

    // index
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);

    // status
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);
}

void annotateMPITestany(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request array
    F->addParamAttr(1, NoCapture);

    // index
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);

    // flag
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);

    // status
    F->addParamAttr(4, WriteOnly);
    F->addParamAttr(4, NoCapture);
}

void annotateMPIWaitall(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request array
    F->addParamAttr(1, NoCapture);

    // status array
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);
}

void annotateMPITestall(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request array
    F->addParamAttr(1, NoCapture);

    // flag
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);

    // status array
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);
}

void annotateMPIWaitTestsome(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request array
    F->addParamAttr(1, NoCapture);

    // out count
    F->addParamAttr(2, WriteOnly);
    F->addParamAttr(2, NoCapture);

    // indices array
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);

    // status array
    F->addParamAttr(4, WriteOnly);
    F->addParamAttr(4, NoCapture);
}

void annotateMPIProbe(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // status
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);
}

void annotateMPIIprobe(llvm::Function *F) {
    annotateMPIProbe(F);

    // status
    F->addParamAttr(4, WriteOnly);
    F->addParamAttr(4, NoCapture);
}

void annotateMPICancel(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // request
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);
}

void annotateMPITestCancelled(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // status
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

    // flag
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);
}

void annotateMPIStart(llvm::Function *F) {
    // cannot annotate memory, because it leads to accessing captured pointers
    F->setDoesNotFreeMemory();

    // request
    F->addParamAttr(0, NoCapture);
}

void annotateMPIStartall(llvm::Function *F) {
    // cannot annotate memory, because it leads to accessing captured pointers
    F->setDoesNotFreeMemory();

    // request array
    F->addParamAttr(1, NoCapture);
}
