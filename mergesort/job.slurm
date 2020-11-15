#!/bin/bash

#SBATCH --time=00:10:00
#SBATCH -N 1	# nodes 
#SBATCH -n 4 	# processors
#SBATCH --mem=64gb
#SBATCH -p classc -A sxg125_csds438

module load gcc

cp merge_sort.c $PFSDIR
cd $PFSDIR
gcc -o mergesortOmp -fopenmp merge_sort.c
time ./mergesortOmp
cp * $SLURM_SUBMIT_DIR
