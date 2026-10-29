#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = atoi(argv[1]);                  // number of ints
    int *buf = malloc(n * sizeof(int));
    double total = 0;

    for (int t = 1; t <= 3; t++) {
        if (rank == 0) for (int i = 0; i < n; i++) buf[i] = i;

        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();

        if (rank == 0) MPI_Send(buf, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
        else           MPI_Recv(buf, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Barrier(MPI_COMM_WORLD);
        double time = MPI_Wtime() - start;
        total += time;

        if (rank == 0) printf("Trial %d: %f s\n", t, time);
        if (rank == 1 && t == 1) printf("First=%d Last=%d\n", buf[0], buf[n-1]);
    }
    if (rank == 0) printf("Average: %f s\n", total / 3);

    free(buf);
    MPI_Finalize();
    return 0;
}