/*

Author: Colin Causey

*/

// Note that merge and mergeSort are based on the implementation from the Walls and Mirrors book.

/* C implementation MergeSort with OpenMP */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <stdbool.h> 
#include <omp.h>

//const int ARR_SIZE = 1000000;
//const int ARR_SIZE = 10000000;
const int ARR_SIZE = 1000;
//const int ARR_SIZE = 10000000;
//const int ARR_SIZE = 40000000;
//const int ARR_SIZE = 100000000;

void exchange(int* a, int* b)
{
  int temp;
  temp = *a;
  *a = *b;
  *b = temp;

  return;
}
int max(int x, int y)
{
  if(x >= y)
    {
      return x;
    }
  else
    {
      return y;
    }
}
int BINARY_SEARCH(int x, int T[], int p, int r)
{
  int low = p;
  int high = max(p, r + 1);
  while(low < high)
    {
      int mid = (low + high) / 2;
      if(x <= T[mid])
	{
	  high = mid;
	}
      else
	{
	  low = mid + 1;
	}
    }
  return high;
}
void P_MERGE(int T[], int p1, int r1, int p2, int r2, int A[], int p3)
{
  int n1 = r1 - p1 + 1;
  int n2 = r2 - p2 + 1;
  if(n1 < n2) // ensure that n1 >= n2
    {
      exchange(&p1, &p2);
      exchange(&r1, &r2);
      exchange(&n1, &n2);
    }
  if(n1 == 0) // both empty?
    {
      return;
    }
  else
    {
      int q1 = (p1 + r1) / 2;
      int q2 = BINARY_SEARCH(T[q1], T, p2, r2);
      int q3 = p3 + (q1 - p1) + (q2 - p2);
      A[q3] = T[q1];
      
      //spawn
      P_MERGE(T, p1, q1 - 1, p2, q2 - 1, A, p3);
      P_MERGE(T, q1 + 1, r1, q2, r2, A, q3 + 1);
      //sync
    }
}
void P_MERGE_SORT(int A[], int p, int r, int B[], int s)
{
  int n = r - p + 1;
  if(n == 1) // 0
    {
      B[s] = A[p];
    }
  else
    {
      int* T;
      T = (int *)malloc(sizeof(int)*n);
      //int T[n];
      int q = (p + r) / 2;
      int q_ = q - p + 1;
      //spawn
      //#pragma omp task  ?  Or nested parallelism?
      P_MERGE_SORT(A, p, q, T, 1); // 0 = 1
      P_MERGE_SORT(A, q + 1, r, T, q_ + 1);
      //sync
      P_MERGE(T, 1, q_, q_ + 1, n, B, s);
      
      //free(T);
    }
}
void printArray(int theArray[], int first, int last)
{
  //std::cout << "{ ";
  printf("{ ");
  int i = first;
  for (i = first; i <= last; i++)
    {
      //std::cout << theArray[i] << " ";
      printf("%d ", theArray[i]);
    }
  //std::cout << "}";
  printf("}");
}
void populateArray(int theArray[], int first, int last)
{
  int i = 0;
  for(i = first; i <= last; i++)
    {
      theArray[i] = i + 1;
    }
}
void populateArrayRandom(int theArray[], int first, int last)
{
  // Fill array with random numbers
  srand(0);
  int i;
  for (i = 0; i <= last; i++) 
    {
      theArray[i] = (rand() % (last + 1)); // random numbers between 1 and (size + 1)
    }
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

int main()
{
  // https://stackoverflow.com/questions/2279052/increase-stack-size-in-linux-with-setrlimit
  // 64L * 1024L * 1024L --> min stack size = 64 Mb
  
  /*
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
  */

  int first = 0;
  int last = ARR_SIZE - 1;
  //int theArray[ARR_SIZE];
  int* A; // array to sort
  // allocate on heap instead of stack to avoid seg faults for large arrays
  A = (int *)malloc(sizeof(int)*ARR_SIZE);
  populateArrayRandom(A, first, last);
  //std::random_shuffle(&theArray[first], &theArray[last]);

  //std::cout << std::endl;
  //std::cout << "Unsorted Array:" << std::endl;
  //printArray(theArray, first, last);
  //std::cout << std::endl;

  /*
  #pragma omp parallel default(none) shared(theArray, first, last)
  {
    #pragma omp single nowait
    { mergeSort(theArray, first, last); }
  }
  */
  int* B; // output subarray
  //B = (int *)malloc(sizeof(int)*ARR_SIZE);
  //omp_set_num_threads(omp_num_procs());
  //omp_set_dynamic(false);
  //omp_num_procs();
  //printf("Number of processors: %d", omp_num_procs());
  //int threads = 8;
  //mergeSort(theArray, first, last, tempArray, threads);

  int p = 0;
  int r = ARR_SIZE - 1;
  int s = 0; // number of threads - 1???
  B = (int *)malloc(sizeof(int)*ARR_SIZE);
  omp_set_nested(true); // enable nested parallelism
  P_MERGE_SORT(A, p, r, B, s);

  //std::cout << "Sorted Array:" << std::endl;
  //printArray(B, first, last);
  //printf("\n");
  //std::cout << std::endl << std::endl;

  if(validateSort(B, first, last))
    {
      printf("Array is correctly sorted.");
    }
  else
    {
      printf("Array is NOT correctly sorted.");
    }

  //delete[] theArray;
  free(A);
  free(B);
  return 0;
}
