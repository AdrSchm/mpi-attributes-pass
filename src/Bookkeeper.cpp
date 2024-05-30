#include "Bookkeeper.hpp"

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"

#include <llvm/Support/raw_ostream.h>
#include <unordered_map>

void Bookkeeper::annotateModule(const llvm::Module &M) {
    // iterate through all functions and count MPI declarations and calls
    for (auto &func : M.functions()) {
        if (func.getName().starts_with("MPI_")) {
            int numberCalls = func.getNumUses();
            usedMPICalls += numberCalls;
            allFuncts[&func] = numberCalls;
        }
    }
}

void Bookkeeper::report() {
    int amountAnnotated = isAnnotated.size();
    llvm::outs() << "Annotation report:\n";
    llvm::outs() << "\tTotal used MPI functions:\t\t" << allFuncts.size() << "\n";
    llvm::outs() << "\tTotal calls to MPI functions:\t\t" << usedMPICalls << "\n";
    llvm::outs() << "\tTotal annotated declarations:\t\t" << amountAnnotated << "\n";
    llvm::outs() << "\tTotal calls to annotated functions:\t" << annotatedCalls << "\n\n";
    if (!isAnnotated.empty()) {
        llvm::outs() << "\tAnnotated function:\tAmount of calls\n";
        for (auto &[function, amountCalls] : allFuncts) {
            if (isAnnotated[function]) {
                llvm::outs() << "\t" << function->getName() << ":\t\t" << amountCalls << "\n";
            }
        }
        llvm::outs() << "\n";
    }
    if (amountAnnotated != allFuncts.size()) {
        llvm::outs() << "\tNot-annotated function:\tAmount of calls\n";
        for (auto &[function, amountCalls] : allFuncts) {
            if (!isAnnotated[function]) {
                llvm::outs() << "\t" << function->getName() << ":\t\t" << amountCalls << "\n";
            }
        }
        llvm::outs() << "\n";
    }
}