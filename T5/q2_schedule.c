// Q2: scheduling with 2 threads. f(i) takes i milliseconds (we use usleep).
// Compile: gcc -fopenmp q2_schedule.c -o q2      Run: ./q2
#include <stdio.h>

#include <omp.h>



#include <unistd.h>
#define n 40                                  // small n so the program runs fast

void f(int i) { 
    usleep(i * 1000);
}         // f(i) takes i ms

int main() {
    double start;
    int i;

    // 1) schedule(static): two big chunks
    start = omp_get_wtime();
    #pragma omp parallel num_threads(2)
    {
        #pragma omp for schedule(static)
        for (i = 1; i < n; i++) f(i);
        #pragma omp for schedule(static)
        for (i = 1; i < n; i++) f(n - i);
    }
    printf("static          : %.2f s\n", omp_get_wtime() - start);

    // 2) schedule(static,1): iterations handed out one by one, round robin
    start = omp_get_wtime();
    #pragma omp parallel num_threads(2)
    {
        #pragma omp for schedule(static, 1)
        for (i = 1; i < n; i++) f(i);
        #pragma omp for schedule(static, 1)
        for (i = 1; i < n; i++) f(n - i);
    }
    printf("static,1        : %.2f s\n", omp_get_wtime() - start);

    // 3) schedule(dynamic,1): a free thread takes the next iteration
    start = omp_get_wtime();
    #pragma omp parallel num_threads(2)
    {
        #pragma omp for schedule(dynamic, 1)
        for (i = 1; i < n; i++) f(i);
        #pragma omp for schedule(dynamic, 1)
        for (i = 1; i < n; i++) f(n - i);
    }
    printf("dynamic,1       : %.2f s\n", omp_get_wtime() - start);

    // 4) static + nowait: no waiting between the two loops
    start = omp_get_wtime();
    #pragma omp parallel num_threads(2)
    {
        #pragma omp for schedule(static) nowait
        for (i = 1; i < n; i++) f(i);
        #pragma omp for schedule(static) nowait
        for (i = 1; i < n; i++) f(n - i);
    }
    printf("static + nowait : %.2f s\n", omp_get_wtime() - start);

    printf("\nTheory (n=%d): static = 3n^2/4 = %d ms, others = n^2/2 = %d ms\n",
           n, 3 * n * n / 4, n * n / 2);
    return 0;
}
