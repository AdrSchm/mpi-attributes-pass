#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 16
#define ITER_TO_PRINT 1000
int main(int argc, char **argv) {
    int rank;
    int size;
    MPI_Init(NULL, NULL);

    double starttime, endtime;
    starttime = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Started %d processes\n", size);
    }

    int num_iters = 1000000;

    double array[ARRAY_SIZE] = {0};

    starttime = MPI_Wtime();
    for (int n = 0; n < num_iters; ++n) {
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = array[i] + (n * rank);
        }
        if (n % ITER_TO_PRINT == 0 && rank == 0) {
            printf("Iter %d : %f\n", n, array[0]);
        }
    }
    endtime = MPI_Wtime();
    double delta_t = endtime - starttime;

    if (rank == 0) {
        MPI_Reduce(MPI_IN_PLACE, array, 16, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(MPI_IN_PLACE, &delta_t, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        printf("Finished: %f\n", array[0]);
        printf("Slowest Process: %f s\n", delta_t);
    } else {
        MPI_Reduce(array, NULL, 16, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&delta_t, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
