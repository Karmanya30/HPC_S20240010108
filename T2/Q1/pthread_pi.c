#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>



typedef struct
{
    long long points;
    long long inside;
    unsigned int seed;
} ThreadData;

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void *calculate(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    data->inside = 0;

    for (long long i = 0; i < data->points; i++)
    {
        double x = (double)rand_r(&data->seed) / RAND_MAX;
        double y = (double)rand_r(&data->seed) / RAND_MAX;

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

    double start = get_time();

    for (int i = 0; i < num_threads; i++)
    {
        data[i].points = points_per_thread;
        data[i].seed = time(NULL) + i;

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

    double end = get_time();

    double pi = 4.0 * total_inside / n;

   double execution_time = end - start;

    printf("\nNumber of points = %lld\n", n);
    printf("Number of threads = %d\n", num_threads);
    printf("Points inside circle = %lld\n", total_inside);
    printf("Estimated Pi = %lf\n", pi);
    printf("Execution time = %lf seconds\n", execution_time);

    return 0;
}