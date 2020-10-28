/*

Author: Colin Causey

*/

// Note that merge and mergeSort are taken from the Walls and Mirrors book.

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sys/resource.h>
#include <stdio.h>

//const int ARR_SIZE = 1000000;
const int ARR_SIZE = 100000000;

void merge(int theArray[], int first, int mid, int last)
{
  int tempArray[ARR_SIZE];

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
}
void mergeSort(int theArray[], int first, int last)
{
  if(first < last)
    {
      int mid = first + (last - first) / 2;
      mergeSort(theArray, first, mid);
      mergeSort(theArray, mid + 1, last);
      merge(theArray, first, mid, last);
    }
}
void printArray(int theArray[], int first, int last)
{
  std::cout << "{ ";
  for (int i = first; i <= last; i++)
    {
      std::cout << theArray[i] << " ";
    }
  std::cout << "}";
}
void populateArray(int theArray[], int first, int last)
{
  for(int i = first; i <= last; i++)
    {
      theArray[i] = i + 1;
    }
}

int main()
{
  // https://stackoverflow.com/questions/2279052/increase-stack-size-in-linux-with-setrlimit
  // 64L * 1024L * 1024L --> min stack size = 64 Mb
  const rlim_t kStackSize = (64L * 1024L * 1024L) * 16;
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

  int first = 0;
  int last = ARR_SIZE - 1;
  //int theArray[ARR_SIZE];
  int* theArray = new int[ARR_SIZE]; // allocate on heap instead of stack to avoid seg faults for large arrays
  populateArray(theArray, first, last);
  std::random_shuffle(&theArray[first], &theArray[last]);
  //std::cout << std::endl;
  //std::cout << "Unsorted Array:" << std::endl;
  //printArray(theArray, first, last);
  //std::cout << std::endl;

  mergeSort(theArray, first, last);

  //std::cout << "Sorted Array:" << std::endl;
  //printArray(theArray, first, last);
  //std::cout << std::endl << std::endl;

  delete[] theArray;
  
  return 0;
}
