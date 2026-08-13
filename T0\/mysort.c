#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 18250

// Swap two integers
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble Sort
void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Partition function for Quick Sort
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quick Sort
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main()
{
    int *original = (int *)malloc(SIZE * sizeof(int));
    int *bubble = (int *)malloc(SIZE * sizeof(int));
    int *quick = (int *)malloc(SIZE * sizeof(int));

    if (original == NULL || bubble == NULL || quick == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    srand(time(NULL));

    // Generate random temperature values (-10 to 49)
    for (int i = 0; i < SIZE; i++)
    {
        original[i] = rand() % 60 - 10;
    }

    // Copy original data
    memcpy(bubble, original, SIZE * sizeof(int));
    memcpy(quick, original, SIZE * sizeof(int));

    clock_t start, end;

    // Bubble Sort Timing
    start = clock();
    bubbleSort(bubble, SIZE);
    end = clock();

    double bubbleTime = (double)(end - start) / CLOCKS_PER_SEC;

    // Quick Sort Timing
    start = clock();
    quickSort(quick, 0, SIZE - 1);
    end = clock();

    double quickTime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Number of Temperature Records : %d\n\n", SIZE);

    printf("Bubble Sort Execution Time : %.6f seconds\n", bubbleTime);
    printf("Quick Sort Execution Time  : %.6f seconds\n", quickTime);

    free(original);
    free(bubble);
    free(quick);

    return 0;
}