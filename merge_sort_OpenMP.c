/* OpenMP Parallel Merge Sort
   Implementation based on Copyright (C) 2011  Atanas Radenski, Introduction to Algorithms, and Walls and Mirrors
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <omp.h>

// Arrays size <= SMALL switches to insertion sort
#define SMALL    32

//extern double get_time (void);
void merge (int a[], int size, int temp[]);
void insertion_sort (int a[], int size);
void mergesort_serial (int a[], int size, int temp[]);
void mergesort_parallel_omp (int a[], int size, int temp[], int threads);
void run_omp (int a[], int size, int temp[], int threads);
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
  printf ("Array size = %d\nProcesses = %d\nProcessors = %d\n",
	  size, threads, processors);
  if (threads > processors)
    {
      printf
	("Warning: %d threads requested, will run_omp on %d processors available\n",
	 threads, processors);
      omp_set_num_threads (threads);
    }
  int max_threads = omp_get_max_threads ();	// Max available threads
  if (threads > max_threads)	// Requested threads are more than max available
    {
      printf ("Error: Cannot use %d threads, only %d threads available\n",
	      threads, max_threads);
      return 1;
    }
  // Array allocation
  int *a = malloc (sizeof (int) * size);
  int *temp = malloc (sizeof (int) * size);
  if (a == NULL || temp == NULL)
    {
      printf ("Error: Could not allocate array of size %d\n", size);
      return 1;
    }
  // Random array initialization
  int i;
  srand (314159);
  for (i = 0; i < size; i++)
    {
      a[i] = rand () % size;
    }
  // Sort
  double start = omp_get_wtime();
  run_omp (a, size, temp, threads);
  double end = omp_get_wtime();
  printf ("Execution Time = %.4f\n",
  	  end - start);
  // Result check
  for (i = 1; i < size; i++)
    {
      if (!(a[i - 1] <= a[i]))
	{
	  printf ("Implementation error: a[%d]=%d > a[%d]=%d\n", i - 1,
		  a[i - 1], i, a[i]);
	  return 1;
	}
    }
  puts ("-Success-");
  return 0;
}

// Driver
void
run_omp (int a[], int size, int temp[], int threads)
{
  // Enable nested parallelism, if available
  omp_set_nested (1);
  // Parallel mergesort
  mergesort_parallel_omp (a, size, temp, threads);
}

// OpenMP merge sort with given number of threads
void
mergesort_parallel_omp (int a[], int size, int temp[], int threads)
{
  if (threads == 1)
    {
//        printf("Thread %d begins serial merge sort\n", omp_get_thread_num());
      mergesort_serial (a, size, temp);
    }
  else if (threads > 1)
    {
#pragma omp parallel sections
      {
//                      printf("Thread %d begins recursive section\n", omp_get_thread_num());
#pragma omp section
	{			//printf("Thread %d begins recursive call\n", omp_get_thread_num());
	  mergesort_parallel_omp (a, size / 2, temp, threads / 2);
	}
#pragma omp section
	{			//printf("Thread %d begins recursive call\n", omp_get_thread_num());
	  mergesort_parallel_omp (a + size / 2, size - size / 2,
				  temp + size / 2, threads - threads / 2);
	}
      }
      // Thread allocation is implementation dependent
      // Some threads can execute multiple sections while others are idle 
      // Merge the two sorted sub-arrays through temp
      merge (a, size, temp);
    }
  else
    {
      printf ("Error: %d threads\n", threads);
      return;
    }
}

void
mergesort_serial (int a[], int size, int temp[])
{
  ///*
  // Switch to insertion sort for small arrays
  if (size <= SMALL)
    {
      insertion_sort (a, size);
      return;
    }
  //*/
  mergesort_serial (a, size / 2, temp);
  mergesort_serial (a + size / 2, size - size / 2, temp);
  // Merge the two sorted subarrays into a temp array
  merge (a, size, temp);
}

void
merge (int a[], int size, int temp[])
{
  int i1 = 0;
  int i2 = size / 2;
  int tempi = 0;
  while (i1 < size / 2 && i2 < size)
    {
      if (a[i1] < a[i2])
	{
	  temp[tempi] = a[i1];
	  i1++;
	}
      else
	{
	  temp[tempi] = a[i2];
	  i2++;
	}
      tempi++;
    }
  while (i1 < size / 2)
    {
      temp[tempi] = a[i1];
      i1++;
      tempi++;
    }
  while (i2 < size)
    {
      temp[tempi] = a[i2];
      i2++;
      tempi++;
    }
  // Copy sorted temp array into main array, a
  memcpy (a, temp, size * sizeof (int));
}

void
insertion_sort (int a[], int size)
{
  int i;
  for (i = 0; i < size; i++)
    {
      int j, v = a[i];
      for (j = i - 1; j >= 0; j--)
	{
	  if (a[j] <= v)
	    break;
	  a[j + 1] = a[j];
	}
      a[j + 1] = v;
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
