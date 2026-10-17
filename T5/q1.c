#include <stdio.h>
#include <omp.h>

#define N 100000   
#define M 8        

double f(int i) { return 1.0; }   

int main() {
    int indices[N];
    double v[M];
    int i, k;

    for (i = 0; i < N; i++) indices[i] = i % M;   // indices repeat!

    // ---- Code 1: no protection (Overwrite issue) ----
    for (k = 0; k < M; k++) v[k] = 0;
    #pragma omp parallel for
    for (i = 0; i < N; i++)
        v[indices[i]] += f(i);
    printf("Code 1 (no protection): v[0] = %.0f   (correct value is %d)\n", v[0], N / M);

    // ---- Code 2: critical ----
    for (k = 0; k < M; k++) v[k] = 0;
    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        #pragma omp critical
        v[indices[i]] += f(i);
    }
    printf("Code 2 (critical)     : v[0] = %.0f   (correct value is %d)\n", v[0], N / M);
    return 0;
}
