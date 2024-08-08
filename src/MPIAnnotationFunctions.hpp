#ifndef MPI_ANNOTATION_FUNCTIONS_HPP_
#define MPI_ANNOTATION_FUNCTIONS_HPP_

#include "llvm/IR/Function.h"

// Generate the list of function declarations from the list in Annotations.def
#ifndef X
#define X(_, FUNC) void FUNC(llvm::Function *F);
#include "Annotations.def"
#undef X
#else
#error "Another macro of the same name is already defined somewhere else, use another one!"
#endif

#endif
