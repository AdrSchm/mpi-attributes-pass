#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int *get_ptr();
__attribute__((pure))
int get_int();
void doOtherStuff();

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d %d\n", size, rank);

    if (rank == 0) {
        int msg = get_int();
        MPI_Request req;
        MPI_Status stat;
        MPI_Isend(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);
        doOtherStuff();
        MPI_Wait(&req, &stat);
        printf("%d\n", msg);
    }
    else if (rank == 1) {
        int buf;
        MPI_Request req;
        MPI_Status stat;
        MPI_Irecv(&buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);
        doOtherStuff();
        MPI_Wait(&req, &stat);
        printf("%d\n", buf);
    }
    MPI_Finalize();
    return 0;
}