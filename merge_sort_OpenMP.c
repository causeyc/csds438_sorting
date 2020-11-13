/* OpenMP Parallel Merge Sort
   Implementation based on (1) Copyright (C) 2011  Atanas Radenski, 
                           (2) Introduction to Algorithms, 3rd ed. (pg. 30-40, 797-804)
                           (3) Data Abstraction and Problem Solving with C++: Walls and Mirrors, 7th ed. (pg. 333-337)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <omp.h>

// Arrays size <= SMALL switches to insertion sort
#define SMALL    32

//extern double get_time (void);
void merge (int A[], int size, int temp[]);
void insertion_sort (int A[], int size);
void mergesort_serial (int A[], int size, int temp[]);
void mergesort_parallel_omp (int A[], int size, int temp[], int threads);
void run_omp (int A[], int size, int temp[], int threads);
void allocateStackSpace();
int main (int argc, char *argv[]);

int
main (int argc, char *argv[])
{
  puts ("-OpenMP Recursive Mergesort-\t");
  // Check arguments
  if (argc != 3)		/* argc must be 3 for proper execution! */
    {
      printf ("Usage: %s array-size number-of-threads\n", argv[0]);
      return 1;
    }
  int index;
  const int TRIALS = 100;
  double totalTime = 0.0000;

  for (index = 1; index <= TRIALS; index++)
  {
    // Get arguments
    int size = atoi (argv[1]);	// Array size 
    int threads = atoi (argv[2]);	// Requested number of threads
    // Check nested parallelism availability
    omp_set_nested (1);
    if (omp_get_nested () != 1)
      {
        puts ("Warning: Nested parallelism desired but unavailable");
      }
    // Check processors and threads
    int processors = omp_get_num_procs ();	// Available processors
    printf ("Array size = %d\nProcesses = %d\nProcessors = %d\n", size, threads, processors);
    if (threads > processors)
      {
        printf("Warning: %d threads requested, will run_omp on %d processors available\n", threads, processors);
        omp_set_num_threads (threads);
      }
    int max_threads = omp_get_max_threads ();	// Max available threads
    if (threads > max_threads)	// Requested threads are more than max available
      {
        printf ("Error: Cannot use %d threads, only %d threads available\n", threads, max_threads);
        return 1;
      }
    // Array allocation
    int *A = malloc (sizeof (int) * size);
    int *temp = malloc (sizeof (int) * size);
    if (A == NULL || temp == NULL)
      {
        printf ("Error: Could not allocate array of size %d\n", size);
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
    printf ("Execution Time = %.4f\n", end - start);
    // Result check
    for (i = 1; i < size; i++)
      {
        if (!(A[i - 1] <= A[i]))
	        {
	          printf ("Implementation error: A[%d]=%d > A[%d]=%d\n", i - 1,
		        A[i - 1], i, A[i]);
	          return 1;
	        }
      }
    //puts ("-Success-");
    free(A);
    free(temp);
  }
  double averageExecutionTime = totalTime / TRIALS;
  printf ("Average Execution Time (%d trials) = %.4f\n", TRIALS, averageExecutionTime);
  return 0;
}

// Driver
void
run_omp (int A[], int size, int temp[], int threads)
{
  // Enable nested parallelism, if available
  omp_set_nested (1);
  // Parallel mergesort
  mergesort_parallel_omp (A, size, temp, threads);
}

// OpenMP merge sort with given number of threads
void
mergesort_parallel_omp (int A[], int size, int temp[], int threads)
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
	  mergesort_parallel_omp (A + size / 2, size - size / 2,
				  temp + size / 2, threads - threads / 2);
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

void
mergesort_serial (int A[], int size, int temp[])
{
  ///*
  // Switch to insertion sort for small arrays
  if (size <= SMALL)
    {
      insertion_sort (A, size);
      return;
    }
  //*/
  mergesort_serial (A, size / 2, temp);
  mergesort_serial (A + size / 2, size - size / 2, temp);
  // Merge the two sorted subarrays into A temp array
  merge (A, size, temp);
}

void
merge (int A[], int size, int temp[])
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

void
insertion_sort (int A[], int size)
{
  int i;
  for (i = 0; i < size; i++)
    {
      int j, v = A[i];
      for (j = i - 1; j >= 0; j--)
	{
	  if (A[j] <= v)
	    break;
	  A[j + 1] = A[j];
	}
      A[j + 1] = v;
    }
}

void allocateStackSpace()
{
  const rlim_t kStackSize = (64L * 1024L * 1024L) * 128;
  struct rlimit rl;
  int result;
  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
    {
      if (rl.rlim_cur < kStackSize)
        {
	  rl.rlim_cur = kStackSize;
	  result = setrlimit(RLIMIT_STACK, &rl);
	  if (result != 0)
            {
	      fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
}
