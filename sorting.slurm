#!/bin/bash

#SBATCH --time=00:10:00
#SBATCH -N 1	# nodes 
#SBATCH -n 16	# processors
#SBATCH --mem=64gb
#SBATCH -p classc -A sxg125_csds438

module load gcc

#cp merge_sort.c $PFSDIR
cp merge_sort_OpenMP.c $PFSDIR
cd $PFSDIR
gcc -o merge_sort_OpenMP -fopenmp merge_sort_OpenMP.c
for i in 1 2 4 8 16
do
  for j in 10000, 100000, 1000000, 10000000, 100000000 1000000000
  do
      ./merge_sort_OpenMP $j $i
  done
done

cp * $SLURM_SUBMIT_DIR
