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

## Other repository contents

`test` contains programs to showcase possible optimizations in the generated assembly.

`example/sample.c` contains a synthetic example, where the additionally enabled optimizations have a measurable effect on the performance.
