/* C implementation QuickSort with OpenMP */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// A utility function to swap two elements
void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
	array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (int arr[], int low, int high)
{
	int pivot = arr[high]; // pivot
	int i = (low - 1); // Index of smaller element
	int j;
	for (j = low; j <= high- 1; j++)
	{
		// If current element is smaller than the pivot
		if (arr[j] < pivot)
		{
			i++; // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		#pragma omp task default(none) firstprivate(arr, low, pi)
		{ quickSort(arr, low, pi - 1); }
		#pragma omp task default(none) firstprivate(arr, high, pi)
		{ quickSort(arr, pi + 1, high); }
	}
}

// Driver program to test above functions
int main(int argc, char *argv[])
{
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
	#pragma omp parallel default(none) shared(arr, n)
	{
		#pragma omp single nowait
		{ quickSort(arr, 0, n-1); }
	}
	double time_end = omp_get_wtime();

	printf("Sorting time: %f seconds", time_end - time_start);
	return 0;
}
