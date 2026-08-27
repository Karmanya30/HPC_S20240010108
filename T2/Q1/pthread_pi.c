#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
    long long points;
    long long inside;
} ThreadData;

void *calculate(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    data->inside = 0;

    for (long long i = 0; i < data->points; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0)
        {
            data->inside++;
        }
    }

    return NULL;
}

int main()
{
    long long n;
    int num_threads;

    printf("Enter number of points: ");
    scanf("%lld", &n);

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    long long points_per_thread = n / num_threads;

    clock_t start = clock();

    for (int i = 0; i < num_threads; i++)
    {
        data[i].points = points_per_thread;

        if (i == num_threads - 1)
        {
            data[i].points += n % num_threads;
        }

        pthread_create(
            &threads[i],
            NULL,
            calculate,
            &data[i]
        );
    }

    long long total_inside = 0;

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);

        total_inside += data[i].inside;
    }

    clock_t end = clock();

    double pi = 4.0 * total_inside / n;

    double execution_time =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nNumber of points = %lld\n", n);
    printf("Number of threads = %d\n", num_threads);
    printf("Points inside circle = %lld\n", total_inside);
    printf("Estimated Pi = %lf\n", pi);
    printf("Execution time = %lf seconds\n", execution_time);

    return 0;
}