#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int *get_ptr();
__attribute__((pure))
int get_int();

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int msg = get_int();
        int buf;
        MPI_Status stat;
        int *other = get_ptr();
        *other = get_int() * 50;
        MPI_Sendrecv(&msg, 1, MPI_INT, 1, 0, &buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &stat);
        printf("%d %d\n", msg, *other);
    }
    else if (rank == 1) {
        int msg = get_int();
        int buf;
        MPI_Status stat;
        MPI_Sendrecv(&msg, 1, MPI_INT, 0, 0, &buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        printf("%d\n", buf);
    }

    MPI_Finalize();
    return 0;
}