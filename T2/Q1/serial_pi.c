#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main()
{
    long long n;
    long long inside = 0;

    printf("Enter number of points: ");
    scanf("%lld", &n);

    srand(time(NULL));

    double start = get_time();

    for (long long i = 0; i < n; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0)
        {
            inside++;
        }
    }

    double end = get_time();

    double pi = 4.0 * inside / n;
    double error = fabs(pi - 3.141592653589793);
    double execution_time = end - start;

    printf("\nNumber of points = %lld\n", n);
    printf("Points inside circle = %lld\n", inside);
    printf("Estimated Pi = %.12f\n", pi);
    printf("Error = %.12f\n", error);
    printf("Execution time = %.6f seconds\n", execution_time);

    return 0;
}