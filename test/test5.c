#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int *get_ptr();
__attribute__((pure))
int get_int();
void doOtherStuff();

const int bufSize = 10;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("%d %d\n", size, rank);

    int *sendBuf = (int *)malloc(bufSize * sizeof(int));
    int *recvBuf = (int *)malloc(bufSize * sizeof(int) * size);

    for (int i = 0; i < bufSize; ++i) {
        sendBuf[i] = rank;
    }

    MPI_Allgather(sendBuf, bufSize, MPI_INT, recvBuf, bufSize, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < bufSize * size; ++i) {
        printf("%d\n", recvBuf[i]);
    }

    free(sendBuf);
    free(recvBuf);

    MPI_Barrier(MPI_COMM_WORLD);

    int *buffer = (int *)malloc(bufSize * sizeof(int) * size);

    for (int i = 0; i < bufSize; ++i) {
        buffer[rank * bufSize + i] = rank;
    }

    MPI_Allgather(MPI_IN_PLACE, bufSize, MPI_INT, buffer, bufSize, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < bufSize * size; ++i) {
        printf("%d\n", recvBuf[i]);
    }

    free(buffer);

    MPI_Finalize();
    return 0;
}