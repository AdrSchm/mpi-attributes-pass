#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int *get_ptr();
int get_int();

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size = 5;
    int rank = 5;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d %d\n", size, rank);

    if (rank == 0) {
        int msg = get_int();
        MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("%d\n", msg);
    }
    else if (rank == 1) {
        int buf = get_int();
        MPI_Status stat;
        buf = buf * 50;
        MPI_Recv(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    }
    return 0;
}

// To compile:
// clang -O1 -Xclang -disable-llvm-passes -S -emit-llvm -fno-discard-value-names ../test/test.c -o ../test/test.ll $(mpicc --showme:compile)

// To run pass, for legacy and new pass manager respectively:
// opt -S --load ./MPIAttrs.so --bugpoint-enable-legacy-pm --mpiattrs ../test/test.ll
// opt -S -load-pass-plugin=./MPIAttrs.so -passes=mpiattrs ../test/test.ll

// All in one:
// clang -fpass-plugin=./MPIAttrs.so -S -O3 ../test/test.c -o ../test/test_pass.s $(mpicc --showme:compile)