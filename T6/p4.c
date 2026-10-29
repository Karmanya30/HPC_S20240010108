// Run: mpirun -np 4 ./p4_ring
#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int previous = (rank - 1 + size) % size;
    int send_val = rank * 10, recv_val;

    // MPI_Sendrecv avoids deadlock (all-send-first could hang for large messages)
    MPI_Sendrecv(&send_val, 1, MPI_INT, next, 0,
                 &recv_val, 1, MPI_INT, previous, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d received %d from rank %d\n", rank, recv_val, previous);
    MPI_Finalize();
    return 0;
}