// Run: mpirun -np 4 ./p5_grid [columns]   (default 2 columns -> 2x2 grid)
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int C = (argc > 1) ? atoi(argv[1]) : 2;
    int R = size / C;
    int row = rank / C, col = rank % C;

    // neighbor ranks, MPI_PROC_NULL if outside the grid
    int up    = (row > 0)     ? rank - C : MPI_PROC_NULL;
    int down  = (row < R - 1) ? rank + C : MPI_PROC_NULL;
    int left  = (col > 0)     ? rank - 1 : MPI_PROC_NULL;
    int right = (col < C - 1) ? rank + 1 : MPI_PROC_NULL;

    int me = rank, from_up = -1, from_down = -1, from_left = -1, from_right = -1;
    // send my rank in one direction, receive from the opposite one (PROC_NULL = no-op)
    MPI_Sendrecv(&me, 1, MPI_INT, down,  0, &from_up,    1, MPI_INT, up,    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&me, 1, MPI_INT, up,    1, &from_down,  1, MPI_INT, down,  1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&me, 1, MPI_INT, right, 2, &from_left,  1, MPI_INT, left,  2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&me, 1, MPI_INT, left,  3, &from_right, 1, MPI_INT, right, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d (row %d, col %d): up=%d down=%d left=%d right=%d  (-1 = none)\n",
           rank, row, col, from_up, from_down, from_left, from_right);
    MPI_Finalize();
    return 0;
}