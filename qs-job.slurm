#!/bin/bash

#SBATCH --time=00:10:00
#SBATCH -N 1
#SBATCH -c 8
#SBATCH -n 2
#SBATCH --mem=16gb
#SBATCH -p classc -A sxg125_csds438

echo "# Elements: 40000000"
echo "--"

for i in 1 2 4 8 16
do
  echo "Number of OpenMP threads: $i"
  export OMP_NUM_THREADS=$i
  time ./quicksortOMP
done
