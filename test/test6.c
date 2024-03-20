#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int *get_ptr();
__attribute__((pure))
int get_int();
void doOtherStuff();

const int bufSize = 100;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d %d\n", size, rank);
    MPI_Request req;

    int *sendBuf = (int *)malloc(bufSize * sizeof(int));
    int *recvBuf = (int *)malloc(bufSize * sizeof(int));

    for (int i = 0; i < bufSize; ++i) {
        sendBuf[i] = rank;
    }

    MPI_Iallreduce(sendBuf, recvBuf, bufSize, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &req);
    doOtherStuff();
    MPI_Wait(&req, MPI_STATUS_IGNORE);

    for (int i = 0; i < bufSize; ++i) {
        printf("%d\n", recvBuf[i]);
    }

    free(recvBuf);

    MPI_Barrier(MPI_COMM_WORLD);


    MPI_Allreduce(MPI_IN_PLACE, sendBuf, bufSize, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < bufSize; ++i) {
        printf("%d\n", sendBuf[i]);
    }

    free(sendBuf);

    MPI_Finalize();
    return 0;
}