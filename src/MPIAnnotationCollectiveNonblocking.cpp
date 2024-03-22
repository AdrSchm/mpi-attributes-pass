#include "MPIAnnotationFunctions.hpp"

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

void annotateMPIIbarrier(llvm::Function *F) {
    // behaves like barrier but with additional request parameter
    annotateMPIBarrier(F);

    // request
    F->addParamAttr(1, WriteOnly);
    F->addParamAttr(1, NoCapture);
}

void annotateMPIIbcast(llvm::Function *F) {
    // behaves like broadcast with additional request parameter
    annotateMPIBcast(F);

    // request
    F->addParamAttr(5, WriteOnly);
    F->addParamAttr(5, NoCapture);
}

void annotateMPIIgather(llvm::Function *F) {
    // behaves like gather with additional request parameter
    annotateMPIGather(F);

    // request
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
}

void annotateMPIIgatherv(llvm::Function *F) {
    // behaves like gatherv with additional request parameter
    annotateMPIGatherv(F);

    // request
    F->addParamAttr(9, WriteOnly);
    F->addParamAttr(9, NoCapture);
}

void annotateMPIIscatter(llvm::Function *F) {
    // behaves like scatter with additional request parameter
    annotateMPIScatter(F);

    // request
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
}

void annotateMPIIscatterv(llvm::Function *F) {
    // behaves like scatterv with additional request parameter
    annotateMPIScatterv(F);

    // request
    F->addParamAttr(9, WriteOnly);
    F->addParamAttr(9, NoCapture);
}

void annotateMPIIallgatherIalltoall(llvm::Function *F) {
    // behaves like allgather/alltoall with additional request parameter
    annotateMPIAllgatherAlltoall(F);

    // request
    F->addParamAttr(7, WriteOnly);
    F->addParamAttr(7, NoCapture);
}

void annotateMPIIallgatherv(llvm::Function *F) {
    // behaves like allgatherv with additional request parameter
    annotateMPIAllgatherv(F);

    // request
    F->addParamAttr(8, WriteOnly);
    F->addParamAttr(8, NoCapture);
}

void annotateMPIIalltoallv(llvm::Function *F) {
    // behaves like alltoallv with additional request parameter
    annotateMPIAlltoallv(F);

    // request
    F->addParamAttr(9, WriteOnly);
    F->addParamAttr(9, NoCapture);
}

void annotateMPIIalltoallw(llvm::Function *F) {
    // behaves like alltoallw with additional request parameter
    annotateMPIAlltoallw(F);

    // request
    F->addParamAttr(9, WriteOnly);
    F->addParamAttr(9, NoCapture);
}

void annotateMPIIreduce(llvm::Function *F) {
    // behaves like reduce with additional request parameter
    annotateMPIReduce(F);

    // request
    F->addParamAttr(7, WriteOnly);
    F->addParamAttr(7, NoCapture);
}

void annotateMPIIallreduce(llvm::Function *F) {
    // behaves like allreduce with additional request parameter
    annotateMPIAllreduce(F);

    // request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
}

void annotateMPIIreduceScatterBlock(llvm::Function *F) {
    // behaves like reduce_scatter_block with additional request parameter
    annotateMPIReduceScatterBlock(F);

    // request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
}

void annotateMPIIreduceScatter(llvm::Function *F) {
    // behaves like reduce_scatter with additional request parameter
    annotateMPIReduceScatter(F);

    // request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
}

void annotateMPIIscanIexscan(llvm::Function *F) {
    // behaves like scan/exscan with additional request parameter
    annotateMPIScanExscan(F);

    // request
    F->addParamAttr(6, WriteOnly);
    F->addParamAttr(6, NoCapture);
}