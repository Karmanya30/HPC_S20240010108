#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main()
{
    int n;

    printf("Enter image size: ");
    scanf("%d", &n);

    unsigned char *image = malloc(n * n * sizeof(unsigned char));
    unsigned char *output = malloc(n * n * sizeof(unsigned char));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            image[i * n + j] = (i + j) % 256;
            output[i * n + j] = 0;
        }
    }

    int kernel[3][3] =
    {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    double start = get_time();

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            int sum = 0;

            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    sum += image[(i + ki) * n + (j + kj)]
                           * kernel[ki + 1][kj + 1];
                }
            }

            if (sum < 0)
                sum = -sum;

            if (sum > 255)
                sum = 255;

            output[i * n + j] = sum;
        }
    }

    double end = get_time();

    printf("\nImage size = %d x %d\n", n,n);
    printf("Execution time = %.6f seconds\n", end - start);

    free(image);
    free(output);

    return 0;
}