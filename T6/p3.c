#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void work() {                               // independent dummy computation
    volatile double x = 0;
    for (long i = 0; i < 200000000L; i++) x += i * 0.5;
}

int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = (argc > 1) ? atoi(argv[1]) : 250000;   // number of ints
    int *buf = malloc(n * sizeof(int));
    MPI_Request req;
    if (rank == 0) for (int i = 0; i < n; i++) buf[i] = i;

    // Blocking: communicate, then compute
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    if (rank == 0) MPI_Send(buf, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
    else           MPI_Recv(buf, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    work();
    MPI_Barrier(MPI_COMM_WORLD);
    double t_block = MPI_Wtime() - start;

    // Non-blocking: start communication, compute, then wait
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    if (rank == 0) MPI_Isend(buf, n, MPI_INT, 1, 1, MPI_COMM_WORLD, &req);
    else           MPI_Irecv(buf, n, MPI_INT, 0, 1, MPI_COMM_WORLD, &req);
    work();
    MPI_Wait(&req, MPI_STATUS_IGNORE);
    MPI_Barrier(MPI_COMM_WORLD);
    double t_nonblock = MPI_Wtime() - start;

    if (rank == 0)
        printf("Blocking: %f s, Non-blocking: %f s\n", t_block, t_nonblock);

    free(buf);
    MPI_Finalize();
    return 0;
}