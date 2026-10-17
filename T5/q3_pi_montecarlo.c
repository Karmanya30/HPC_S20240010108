// Q3: estimate pi with the Monte Carlo method
// Compile: gcc -fopenmp -O2 q3_pi_montecarlo.c -o q3 -lm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main() {
    long long sizes[] = {10, 100, 10000, 1000000, 10000000, 100000000, 1000000000};
    int count = 7;

    printf("N,pi_estimate,abs_error\n");

    for (int s = 0; s < count; s++) {
        long long N = sizes[s];
        long long inside = 0;                 // points inside the circle

        #pragma omp parallel reduction(+:inside)
        {
            // every thread has its OWN seed -> rand_r is thread safe
            unsigned int seed = 1234 + omp_get_thread_num();

            #pragma omp for
            for (long long i = 0; i < N; i++) {
                double x = (double) rand_r(&seed) / RAND_MAX;   // 0..1
                double y = (double) rand_r(&seed) / RAND_MAX;   // 0..1
                if (x * x + y * y <= 1.0)
                    inside++;
            }
        }

        double pi = 4.0 * inside / N;         // pi = 4 * P
        printf("%lld,%.10f,%.10f\n", N, pi, fabs(pi - M_PI));
    }
    return 0;
}
