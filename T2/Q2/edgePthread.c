#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int kernel[3][3] =
{
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
};

typedef struct
{
    unsigned char *image;
    unsigned char *output;
    int n;
    int start;
    int end;
} ThreadData;

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void *process(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->start; i < data->end; i++)
    {
        for (int j = 1; j < data->n - 1; j++)
        {
            int sum = 0;

            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    sum += data->image[(i + ki) * data->n + (j + kj)]
                           * kernel[ki + 1][kj + 1];
                }
            }

            if (sum < 0)
                sum = -sum;

            if (sum > 255)
                sum = 255;

            data->output[i * data->n + j] = sum;
        }
    }

    return NULL;
}

int main()
{
    int n, num_threads;

    printf("Enter image size: ");
    scanf("%d", &n);

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    unsigned char *image = malloc(n * n);
    unsigned char *output = malloc(n * n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            image[i * n + j] = (i + j) % 256;
            output[i * n + j] = 0;
        }
    }

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    int rows = (n - 2) / num_threads;

    double start_time = get_time();

    for (int i = 0; i < num_threads; i++)
    {
        data[i].image = image;
        data[i].output = output;
        data[i].n = n;

        data[i].start = 1 + i * rows;

        if (i == num_threads - 1)
            data[i].end = n - 1;
        else
            data[i].end = 1 + (i + 1) * rows;

        pthread_create(&threads[i], NULL, process, &data[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    double end_time = get_time();

    printf("\nImage size = %d x %d\n", n, n);
    printf("Number of threads = %d\n", num_threads);
    printf("Execution time = %.6f seconds\n",
           end_time - start_time);

    free(image);
    free(output);

    return 0;
}