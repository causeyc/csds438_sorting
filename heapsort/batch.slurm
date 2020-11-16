#!/bin/bash
#SBATCH -N 1
#SBATCH -c 8
#SBATCH -n 2
#SBATCH --mem=64gb
#SBATCH -p classc -A sxg125_csds438
gcc -fopenmp ./quicksort/quicksortOMP.c -o ./quicksort/quicksortOMP
gcc -fopenmp ./heapsort/heapsort.c -o ./heapsort/heapsort
echo -e "----------------------------"
for i in 1 2 4 8 16
do
  for j in 10000 100000 1000000 10000000 100000000 1000000000
  do
    echo -e "Number of OpenMP threads: $i"
    echo -e "Number of elements: $j"
    export OMP_NUM_THREADS=$i
    echo "Quicksort:"
    ./quicksort/quicksortOMP $j
    echo "Heapsort:"
    ./heapsort/heapsort $j
    echo -e "\n----------------------------"
  done
  echo -e "----------------------------"
done
