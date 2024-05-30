#include "MPIAttributesPass.hpp"

#include "Bookkeeper.hpp"
#include "MPIAnnotationFunctions.hpp"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>

Bookkeeper bookkeeper;
// Commandline argument to switch on printing an annotation report
static llvm::cl::opt<bool>
    printReport("print-annotation-report",
                llvm::cl::desc("Print a report on the annotated MPI functions"));

// Create a map, that associates the function names with their corresponding
// annotation function by including them from a separate file
typedef void (*annotationFunct_t)(llvm::Function *);
std::unordered_map<std::string, annotationFunct_t> functionMap = {
#ifndef X
#define X(NAME, FUNC) \
    { #NAME, FUNC }
#include "Annotations.def"
#undef X
#else
#error "Another macro of the same name is already defined somewhere else, use another one!"
#endif
};

/*
   Function that performs the actual work.
   It's separated so it can be called from both the legacy pass and the new pass.
   It works on the principle of testing whether each function exists and then adding attributes
   accordingly
*/
bool runMPIAttributesPass(llvm::Module &M) {
    // only need to do this if we want a report
    if (printReport) {
        bookkeeper.annotateModule(M);
    }
    llvm::outs() << "Starting annotating MPI functions...";

    // iterate through all pairs in the map, check if the function exists and call
    // the corresponding annotation function if possible
    for (auto &[functionName, annotationFunction] : functionMap) {
        if (auto *func = M.getFunction(functionName)) {
            annotationFunction(func);
            bookkeeper.annotateFunction(func);
        }
    }

    llvm::outs() << "Finished annotating MPI functions!\n";
    /*
       BEWARE:  Reports all annotated functions, even those that are removed
                during later optimizations. All code included from header files
                is also counted. When using e.g. OpenMPI that means the C++
                bindings and their use of MPI functions in headers may also be
                also reported, even though they are not part of the users's code
    */
    if (printReport) {
        bookkeeper.report();
    }

    // we potentially did modify the module
    return true;
}

/****************************************************************\
|                                                                |
|   from here on boilerplate code to get it all up and running   |
|                                                                |
\****************************************************************/
llvm::PreservedAnalyses MPIAttributesPass::run(llvm::Module &M,
                                               llvm::ModuleAnalysisManager & /*MAM*/) {
    if (!runMPIAttributesPass(M)) {
        return llvm::PreservedAnalyses::all();
    }
    return llvm::PreservedAnalyses::none();
}

bool LegacyMPIAttributesPass::runOnModule(llvm::Module &M) {
    return runMPIAttributesPass(M);
}

// register pass for legacy pass manager
char LegacyMPIAttributesPass::ID = 0;
static llvm::RegisterPass<LegacyMPIAttributesPass>
    X("mpiattrs", "Adds attributes for MPI functions concerning pointer accesses", false, false);

// register pass for new pass manager
llvm::PassPluginLibraryInfo getMPIAttributesPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "MPIAttributesPass", LLVM_VERSION_STRING,
            [](llvm::PassBuilder &PB) {
                PB.registerPipelineEarlySimplificationEPCallback(
                    [&](llvm::ModulePassManager &MPM, auto) {
                        MPM.addPass(MPIAttributesPass());
                        return true;
                    });
                PB.registerPipelineParsingCallback(
                    [](llvm::StringRef Name, llvm::ModulePassManager &PM,
                       llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                        if (Name == "mpiattrs") {
                            PM.addPass(MPIAttributesPass());
                            return true;
                        }
                        return false;
                    });
            }};
}

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getMPIAttributesPassPluginInfo();
}
