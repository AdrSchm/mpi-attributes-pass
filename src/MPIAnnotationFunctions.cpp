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
    llvm::Argument *bufferArg = F->arg_begin() + 1;
    llvm::AttrBuilder &sretAttr = llvm::AttrBuilder(F->getContext())
                                      .addStructRetAttr(llvm::Type::getInt32Ty(F->getContext()));
    bufferArg->addAttrs(sretAttr);
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
    F->setDoesNotAccessMemory();
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

void annotateMPISend(llvm::Function *F) {
    /*
       This is not exactly true in the strictest sense. It's possible to attach a buffer and then
       manually write there. MPI doesn't specify the behavior in this case, so it is literally
       undefined. Therefore we can assume that a compliant program does not access this buffer, and
       we can treat it as if it was inaccessible.
       This applies to other usages of buffers as well.
    */
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

void annotateMPISendrecv(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // send buffer
    F->addParamAttr(0, ReadOnly);
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // send datatype
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);
#endif

    // receive buffer
    F->addParamAttr(5, WriteOnly);
    F->addParamAttr(5, NoCapture);

#ifdef USE_OPENMPI
    // receive datatype
    F->addParamAttr(7, ReadOnly);
    F->addParamAttr(7, NoCapture);

    // communicator
    F->addParamAttr(10, ReadOnly);
    F->addParamAttr(10, NoCapture);
#endif

    // status
    F->addParamAttr(11, WriteOnly);
    F->addParamAttr(11, NoCapture);
    F->addDereferenceableOrNullParamAttr(11, 12);
}

void annotateMPISendrecvReplace(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // datatype
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // communicator
    F->addParamAttr(7, ReadOnly);
    F->addParamAttr(7, NoCapture);
#endif

    // status
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
    F->addDereferenceableOrNullParamAttr(8, 12);
}