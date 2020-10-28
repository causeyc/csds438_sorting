#!/bin/bash

#SBATCH --time=00:10:00
#SBATCH -N 1	# nodes 
#SBATCH -n 2 	# processors
#SBATCH --mem=8gb
#SBATCH -p classc -A sxg125_csds438

module load gcc

cp merge_sort.cpp $PFSDIR
cd $PFSDIR
g++ -o mergeSort merge_sort.cpp
time ./mergeSort
cp * $SLURM_SUBMIT_DIR