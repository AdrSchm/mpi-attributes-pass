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
    MPI_Info info;

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

    MPI_Send_init(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Bsend_init(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Ssend_init(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Rsend_init(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Recv_init(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);

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

    MPI_Start(&req);
    MPI_Startall(1, &req);

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
    MPI_Reduce(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Reduce_local(&buf, &buf, 1, MPI_INT, MPI_SUM);
    MPI_Reduce_scatter_block(&buf, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Reduce_scatter(&buf, &buf, &buf, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Scan(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Exscan(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    MPI_Ibarrier(MPI_COMM_WORLD, &req);
    MPI_Ibcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD, &req);
    MPI_Igather(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, 0, MPI_COMM_WORLD, &req);
    MPI_Igatherv(MPI_IN_PLACE, 1, MPI_INT, &buf, &buf, &buf, MPI_INT, 0, MPI_COMM_WORLD, &req);
    MPI_Iscatter(&buf, 1, MPI_INT, MPI_IN_PLACE, 1, MPI_INT, 0, MPI_COMM_WORLD, &req);
    MPI_Iscatterv(&buf, &buf, &buf, MPI_INT, MPI_IN_PLACE, 1, MPI_INT, 0, MPI_COMM_WORLD, &req);
    MPI_Iallgather(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, MPI_COMM_WORLD, &req);
    MPI_Iallgatherv(MPI_IN_PLACE, 1, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD, &req);
    MPI_Ialltoall(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, MPI_COMM_WORLD, &req);
    MPI_Ialltoallv(MPI_IN_PLACE, &buf, &buf, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD, &req);
    MPI_Ialltoallw(&buf, &buf, &buf, types, &buf, &buf, &buf, types, MPI_COMM_WORLD, &req);
    MPI_Ireduce(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD, &req);
    MPI_Iallreduce(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &req);
    MPI_Ireduce_scatter_block(&buf, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &req);
    MPI_Ireduce_scatter(&buf, &buf, &buf, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &req);
    MPI_Iscan(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &req);
    MPI_Iexscan(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, &req);

    // MPI_Barrier_init(MPI_COMM_WORLD, info, &req);
    // MPI_Bcast_init(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD, info, &req);
    // MPI_Gather_init(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, 0, MPI_COMM_WORLD, info, &req);
    // MPI_Gatherv_init(MPI_IN_PLACE, 1, MPI_INT, &buf, &buf, &buf, MPI_INT, 0, MPI_COMM_WORLD, info, &req);
    // MPI_Scatter_init(&buf, 1, MPI_INT, MPI_IN_PLACE, 1, MPI_INT, 0, MPI_COMM_WORLD, info, &req);
    // MPI_Scatterv_init(&buf, &buf, &buf, MPI_INT, MPI_IN_PLACE, 1, MPI_INT, 0, MPI_COMM_WORLD, info, &req);
    // MPI_Allgather_init(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, MPI_COMM_WORLD, info, &req);
    // MPI_Allgatherv_init(MPI_IN_PLACE, 1, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD, info, &req);
    // MPI_Alltoall_init(MPI_IN_PLACE, 1, MPI_INT, &buf, 1, MPI_INT, MPI_COMM_WORLD, info, &req);
    // MPI_Alltoallv_init(&buf, &buf, &buf, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD, info, &req);
    // MPI_Alltoallv_init(MPI_IN_PLACE, &buf, &buf, MPI_INT, &buf, &buf, &buf, MPI_INT, MPI_COMM_WORLD, info, &req);
    // MPI_Alltoallw_init(&buf, &buf, &buf, types, &buf, &buf, &buf, types, MPI_COMM_WORLD, info, &req);
    // MPI_Alltoallw_init(MPI_IN_PLACE, &buf, &buf, types, &buf, &buf, &buf, types, MPI_COMM_WORLD, info, &req);
    // MPI_Reduce_init(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD, info, &req);
    // MPI_Allreduce_init(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, info, &req);
    // MPI_Reduce_scatter_block_init(&buf, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, info, &req);
    // MPI_Reduce_scatter_init(&buf, &buf, &buf, MPI_INT, MPI_SUM, MPI_COMM_WORLD, info, &req);
    // MPI_Scan_init(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, info, &req);
    // MPI_Exscan_init(MPI_IN_PLACE, &buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD, info, &req);
}