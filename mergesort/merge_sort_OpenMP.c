/* Author: Colin Causey
   OpenMP Parallel Merge Sort
   Implementation based on (1) Copyright (C) 2011  Atanas Radenski, 
                           (2) Introduction to Algorithms, 3rd ed. (pg. 30-40, 797-804)
                           (3) Data Abstraction and Problem Solving with C++: Walls and Mirrors, 7th ed. (pg. 333-337)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <omp.h>

bool validateSort(int theArray[], int first, int last);
void merge (int A[], int size, int temp[]);
void mergesort_serial (int A[], int size, int temp[]);
void mergesort_parallel_omp (int A[], int size, int temp[], int threads);
void run_omp (int A[], int size, int temp[], int threads);
int main (int argc, char *argv[]);

int main (int argc, char *argv[])
{
  puts("-OpenMP Parallel Mergesort-\t");
  // Check arguments
  if (argc != 3)		/* argc must be 3 for proper execution! */
    {
      printf("Usage: %s array-size number-of-threads\n", argv[0]);
      return 1;
    }
  int index;
  const int TRIALS = 5;
  double totalTime = 0.000000;

  for (index = 1; index <= TRIALS; index++)
  {
    // Get arguments
    int size = atoi (argv[1]);	// Array size 
    int threads = atoi (argv[2]);	// Requested number of threads
    // Check nested parallelism availability
    omp_set_nested (1);
    if (omp_get_nested () != 1)
      {
        puts("Warning: Nested parallelism desired but unavailable");
      }
    // Check processors and threads
    int processors = omp_get_num_procs ();	// Available processors
    //printf ("Array size = %d\nProcesses = %d\nProcessors = %d\n", size, threads, processors);
    if (threads > processors)
      {
        printf("Warning: %d threads requested, will run_omp on %d processors available\n", threads, processors);
        omp_set_num_threads (threads);
      }
    int max_threads = omp_get_max_threads ();	// Max available threads
    if (threads > max_threads)	// Requested threads are more than max available
      {
        printf("Error: Cannot use %d threads, only %d threads available\n", threads, max_threads);
        return 1;
      }
    // Array allocation
    int *A = malloc (sizeof (int) * size);
    int *temp = malloc (sizeof (int) * size);
    if (A == NULL || temp == NULL)
      {
        printf("Error: Could not allocate array of size %d\n", size);
        return 1;
      }
    // Random array initialization
    int i;
    srand (314159);
    for (i = 0; i < size; i++)
      {
        A[i] = rand () % size;
      }
    // Sort
    double start = omp_get_wtime();
    run_omp (A, size, temp, threads);
    double end = omp_get_wtime();
    double executionTime = end - start;
    totalTime += executionTime;
    //printf ("Execution Time = %.6f\n", end - start);
    // Validate that array is correctly sorted
    if(validateSort(A, 0, size - 1) == false)
    {
      puts("Error. Array not correctly sorted");
      return 1;
    }
    //puts ("-Success-");
    free(A);
    free(temp);
  }
  double averageExecutionTime = totalTime / TRIALS;
  printf ("Average Execution Time (%d trials) = %.6f\n", TRIALS, averageExecutionTime);
  return 0;
}

// Driver
void run_omp (int A[], int size, int temp[], int threads)
{
  // Enable nested parallelism, if available
  omp_set_nested (1);
  // Parallel mergesort
  mergesort_parallel_omp (A, size, temp, threads);
}

// OpenMP merge sort with given number of threads
void mergesort_parallel_omp (int A[], int size, int temp[], int threads)
{
  if (threads == 1)
    {
//        printf("Thread %d begins serial merge sort\n", omp_get_thread_num());
      mergesort_serial (A, size, temp);
    }
  else if (threads > 1)
    {
#pragma omp parallel sections
      {
//                      printf("Thread %d begins recursive section\n", omp_get_thread_num());
#pragma omp section
	{			//printf("Thread %d begins recursive call\n", omp_get_thread_num());
	  mergesort_parallel_omp (A, size / 2, temp, threads / 2);
	}
#pragma omp section
	{			//printf("Thread %d begins recursive call\n", omp_get_thread_num());
	  mergesort_parallel_omp (A + size / 2, size - size / 2, temp + size / 2, threads - threads / 2);
	}
      }
      // Thread allocation is implementation dependent
      // Some threads can execute multiple sections while others are idle 
      // Merge the two sorted sub-arrays through temp
      merge (A, size, temp);
    }
  else
    {
      printf ("Error: %d threads\n", threads);
      return;
    }
}

void mergesort_serial (int A[], int size, int temp[])
{
  if (size == 1) // one-element arrays are trivially sorted
    {
      return;
    }
  mergesort_serial (A, size / 2, temp);
  mergesort_serial (A + size / 2, size - size / 2, temp);
  // Merge the two sorted subarrays into A temp array
  merge (A, size, temp);
}

void merge (int A[], int size, int temp[])
{
  int i1 = 0;
  int i2 = size / 2;
  int tempi = 0;
  while (i1 < size / 2 && i2 < size)
    {
      if (A[i1] < A[i2])
	{
	  temp[tempi] = A[i1];
	  i1++;
	}
      else
	{
	  temp[tempi] = A[i2];
	  i2++;
	}
      tempi++;
    }
  while (i1 < size / 2)
    {
      temp[tempi] = A[i1];
      i1++;
      tempi++;
    }
  while (i2 < size)
    {
      temp[tempi] = A[i2];
      i2++;
      tempi++;
    }
  // Copy sorted temp array into main array, A
  memcpy (A, temp, size * sizeof (int));
}

bool validateSort(int theArray[], int first, int last)
{
  int i = 0;
  for(i = 0; i <= last - 1; i++)
    {
      if(theArray[i] > theArray[i + 1])
	{
	  return false;
	}
    }
  return true;
}
