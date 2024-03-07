#include "MPIAnnotationFunctions.hpp"

#include "llvm/IR/Argument.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"

// Use shorter names for needed attributes
const auto ReadOnly = llvm::Attribute::AttrKind::ReadOnly;
const auto WriteOnly = llvm::Attribute::AttrKind::WriteOnly;
const auto ReadNone = llvm::Attribute::AttrKind::ReadNone;
const auto NoCapture = llvm::Attribute::AttrKind::NoCapture;

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

    // status or request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
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

    // status or request
    F->addParamAttr(11, WriteOnly);
    F->addParamAttr(11, NoCapture);
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

    // status or request
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
}

void annotateMPIIsend(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

    // buffer
    F->addParamAttr(0, ReadOnly);
    /*
       Technically speaking it does capture the pointer, because it is a nonblocking operation. The
       MPI standard advises to not modify the buffer until the operation gets completed, after which
       the pointer behaves as if it wouldn't have been captured. Therefore we can say that this is
       valid for standard-complying MPI programs.
       The same can be said for the other non-blocking operations.
    */
    F->addParamAttr(0, NoCapture);

#ifdef USE_OPENMPI
    // datatype
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);

    // communicator
    F->addParamAttr(5, ReadOnly);
    F->addParamAttr(5, NoCapture);
#endif

    // request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
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
    F->addParamAttr(0, WriteOnly);
    F->addParamAttr(0, NoCapture);
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

#ifdef USE_OPENMPI
    // communicator
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);
#endif

    // status
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);
}

void annotateMPIIprobe(llvm::Function *F) {
    F->setOnlyAccessesInaccessibleMemOrArgMem();
    F->setDoesNotFreeMemory();

#ifdef USE_OPENMPI
    // communicator
    F->addParamAttr(2, ReadOnly);
    F->addParamAttr(2, NoCapture);
#endif

    // flag
    F->addParamAttr(3, WriteOnly);
    F->addParamAttr(3, NoCapture);

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