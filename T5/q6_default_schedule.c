#include <stdio.h>
#include <omp.h>

int main() {

    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < 16; i++) {
        printf("Iteration %d -> Thread %d\n",
               i, omp_get_thread_num());
    }

    return 0;
}