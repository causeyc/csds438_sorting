#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    // This loop cannot be parallelized since it operates on the whole array
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    // This one can't either for the same reason
    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
	    printf("Incorrect usage. Usage is ./quicksortOMP <number of elements to sort>");
	}
	int n = atoi(argv[1]); // read in number of elements from command line argument
	int *arr;
	arr = (int *)malloc(sizeof(int)*n);

	// Fill array with random numbers
	srand(0);
	int i;
	for (i = 0; i < n; i++) {
		arr[i] = (rand() % n); // random numbers between 1 and n
	}

	double time_start = omp_get_wtime();
    heapSort(arr, n);
    double time_end = omp_get_wtime();
    printf("Sorting time: %f seconds", time_end - time_start);
}