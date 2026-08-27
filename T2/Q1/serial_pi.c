#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    long long n;
    long long inside = 0;

    printf("Enter number of points: ");
    scanf("%lld", &n);

    srand(time(NULL));

    clock_t start = clock();

    for (long long i = 0; i < n; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0)
        {
            inside++;
        }
    }

    clock_t end = clock();

    double pi = 4.0 * inside / n;
    double execution_time =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nNumber of points = %lld\n", n);
    printf("Points inside circle = %lld\n", inside);
    printf("Estimated Pi = %lf\n", pi);
    printf("Execution time = %lf seconds\n", execution_time);

    return 0;
}