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
const int ARR_SIZE = 10000000;
//const int ARR_SIZE = 10000000;
//const int ARR_SIZE = 40000000;
//const int ARR_SIZE = 100000000;

void merge(int* theArray, int first, int mid, int last, int* tempArray)
{
  //int tempArray[ARR_SIZE]; // allocate on stack to avoid expensive free operation for each tempArray generated
  //int* tempArray;
  //tempArray = (int *)malloc(sizeof(int)*ARR_SIZE);

  int first1 = first;
  int last1 = mid;
  int first2 = mid + 1;
  int last2 = last;

  int index = first1;
  while((first1 <= last1) && (first2 <= last2))
    {
      if(theArray[first1] <= theArray[first2])
	{
	  tempArray[index] = theArray[first1];
	  first1++;
	}
      else
	{
	  tempArray[index] = theArray[first2];
	  first2++;
	}
      index++;
    }
  while(first1 <= last1)
    {
      tempArray[index] = theArray[first1];
      first1++;
      index++;
    }
  while(first2 <= last2)
    {
      tempArray[index] = theArray[first2];
      first2++;
      index++;
    }
  for(index = first; index <= last; index++)
    {
      theArray[index] = tempArray[index];
    }

  //free(tempArray);
}
void mergeSort(int* theArray, int first, int last, int* tempArray)
{
  if(first < last)
    {
      int mid = first + (last - first) / 2;
      //#pragma omp task default(none) firstprivate(theArray, first, mid, last)
      ///*
      #pragma omp parallel sections
      {
        #pragma omp section
        {
          mergeSort(theArray, first, mid, tempArray);
        }
        #pragma omp section
        {
          mergeSort(theArray, mid + 1, last, tempArray);
        }
      }
      //*/
      //#pragma omp barrier
      //mergeSort(theArray, first, mid, tempArray);
      //mergeSort(theArray, mid + 1, last, tempArray);
      merge(theArray, first, mid, last, tempArray);
    }
}
void printArray(int theArray[], int first, int last)
{
  //std::cout << "{ ";
  int i = first;
  for (i = first; i <= last; i++)
    {
      //std::cout << theArray[i] << " ";
    }
  //std::cout << "}";
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
  int* theArray;
  // allocate on heap instead of stack to avoid seg faults for large arrays
  theArray = (int *)malloc(sizeof(int)*ARR_SIZE);
  populateArrayRandom(theArray, first, last);
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
  int* tempArray;
  tempArray = (int *)malloc(sizeof(int)*ARR_SIZE);
  //omp_set_num_threads(omp_num_procs());
  //omp_set_dynamic(false);
  //omp_num_procs();
  //printf("Number of processors: %d", omp_num_procs());
  mergeSort(theArray, first, last, tempArray);

  //std::cout << "Sorted Array:" << std::endl;
  //printArray(theArray, first, last);
  //std::cout << std::endl << std::endl;

  if(validateSort(theArray, first, last))
    {
      printf("Array is correctly sorted.");
    }
  else
    {
      printf("Array is NOT correctly sorted.");
    }

  //delete[] theArray;
  free(theArray);
  free(tempArray);
  return 0;
}
