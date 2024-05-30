# LLVM Pass adding Attributes to MPI Functions

To use MPI libraries, function declarations are included via header files. Compilers therefore have no information about the semantics of these functions and are limited in some optimizations that are theoretically possible. Using this LLVM pass during the compilation process, function and parameter attributes are added that describe the memory behavior of commonly used MPI functions, which enables some of the missed optimizations.

## Building with CMake

Requirements:
- CMake >= 3.10
- LLVM 17
- compiler supporting C++17

Example to build using CMake:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Usage

Using Clang the pass can be included with the `-fpass-plugin` flag. As the pass only annotates MPI functions, an MPI application is needed to see any effects.

Example usage of the flag:

    $ clang prog.c -fpass-plugin=/path/to/pass

The pass provides a command line argument to additionally print a report of the performed annotations.
It includes the total amount of MPI function declarations, the total amount of calls to MPI functions, the total amount of annotated MPI function declarations, the total amount of calls to annotated MPI functions, as well as an overview on a per-function basis, how often they are called.

Example usage of the pass, including printing a report:

    $ clang prog.c -fplugin=/path/to/pass -fpass-plugin=/path/to/pass -mllvm -print-annotation-report

Beware that the report includes all annotations and function calls, even those that are removed in later optimizations.

## Extending the Pass

To add attributes to additional functions it is possible to extend the pass.
The following steps are necessary:

1. Add a function with return type `void` and a single parameter of type `llvm::Function*`, that performs the annotation.
2. Make sure that the function declaration is accessible from the code in `src/MPIAttributesPass.cpp`.
3. Extend the list in `src/Attributes.def` with the name of the function-to-annotate and the name of the added annotation function.
4. Recompile the pass.

## Other Repository Contents

`test` contains programs to showcase possible optimizations in the generated assembly.

`example/sample.c` contains a synthetic example, where the additionally enabled optimizations have a measurable effect on the performance.
