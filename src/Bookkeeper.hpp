#ifndef BOOKKEEPER_HPP_
#define BOOKKEEPER_HPP_

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"

#include <unordered_map>

class Bookkeeper {
  public:
    void annotateModule(const llvm::Module &M);
    void annotateFunction(llvm::Function *F);
    void report();

  private:
    int annotatedCalls = 0;
    int usedMPICalls = 0;
    std::unordered_map<const llvm::Function *, int> allFuncts;
    std::unordered_map<const llvm::Function *, bool> isAnnotated;
};

#endif