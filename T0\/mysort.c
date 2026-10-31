
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 18250

// Swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Quick Sort partition
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

// Quick Sort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    int *arr1 = malloc(SIZE * sizeof(int));
    int *arr2 = malloc(SIZE * sizeof(int));

    if (arr1 == NULL || arr2 == NULL) {
        printf("Memory allocation failed\n");
        free(arr1);
        free(arr2);
        return 1;
    }

    srand(42);

    // Generate identical random input
    for (int i = 0; i < SIZE; i++) {
        arr1[i] = rand() % 100000;
        arr2[i] = arr1[i];
    }

    clock_t start, end;
    double time_taken;

    // Bubble Sort
    start = clock();

    bubbleSort(arr1, SIZE);

    end = clock();

    time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Bubble Sort: %.6f seconds\n", time_taken);

    // Quick Sort
    start = clock();

    quickSort(arr2, 0, SIZE - 1);

    end = clock();

    time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Quick Sort: %.6f seconds\n", time_taken);

    free(arr1);
    free(arr2);

    return 0;
}
