/*==================================================================================*\
 *                                                                                  *
 * NO USEFUL PROGRAM! ONLY USED TO SEE IF ALL ATTRIBUTES ARE ANNOTATED AS EXPECTED! *
 *                                                                                  *
\*==================================================================================*/
#include <mpi.h>

int main(int argc, char **argv) {
    int buf;
    MPI_Status stat;
    MPI_Request req;
    MPI_Datatype *types;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &buf);
    MPI_Comm_rank(MPI_COMM_WORLD, &buf);
    MPI_Buffer_attach(&buf, 1);
    MPI_Buffer_detach(&buf, &buf);

    MPI_Send(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Bsend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Ssend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Rsend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    MPI_Sendrecv(&buf, 1, MPI_INT, 0, 0, &buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    MPI_Sendrecv_replace(&buf, 1, MPI_INT, 0, 0, 0, 0, MPI_COMM_WORLD, &stat);

    MPI_Isend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Ibsend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Issend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Irsend(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Irecv(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    // MPI_Isendrecv(&buf, 1, MPI_INT, 0, 0, &buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    // MPI_Isendrecv_replace(&buf, 1, MPI_INT, 0, 0, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Wait(&req, &stat);
    MPI_Test(&req, &buf, &stat);
    MPI_Waitany(1, &req, &buf, &stat);
    MPI_Testany(1, &req, &buf, &buf, &stat);
    MPI_Waitall(1, &req, &stat);
    MPI_Testall(1, &req, &buf, &stat);
    MPI_Waitsome(1, &req, &buf, &buf, &stat);
    MPI_Testsome(1, &req, &buf, &buf, &stat);
    MPI_Probe(0, 0, MPI_COMM_WORLD, &stat);
    MPI_Iprobe(0, 0, MPI_COMM_WORLD, &buf, &stat);
    MPI_Cancel(&req);
    MPI_Test_cancelled(&stat, &buf);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gatherv(MPI_IN_PLACE, 1, MPI_INT, &buf, &buf, &buf, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&buf, 1, MPI_INT, MPI_IN_PLACE, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(&buf, &buf, &buf, MPI_INT, MPI_IN_PLACE, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgatherv(MPI_IN_PLACE, 1, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD);
    MPI_Alltoall(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Alltoallv(&buf, &buf, &buf, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD);
    MPI_Alltoallv(MPI_IN_PLACE, &buf, &buf, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD);
    MPI_Alltoallw(&buf, &buf, &buf, types, &buf, &buf, &buf, types, MPI_COMM_WORLD);
    MPI_Alltoallw(MPI_IN_PLACE, &buf, &buf, types, &buf, &buf, &buf, types, MPI_COMM_WORLD);
}