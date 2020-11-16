# CSDS 438 Final Project - Group #1
In our CSDS 438 HPC final project we measure the effect of adding more processing resources to commonly implemented sorting algorithms.

## Reproduction of Results  

### Mergesort:  

**STATUS:**  Working! 

**RUNNING MERGESORT**  
After Cloning:
To run MergeSort, simply *cd* to the Mergesort file ```cd ./mergeSort``` and submit your the job.slurm job file to the HPC enviroment by running ```sbatch job.slurm```. When the procedure is done, you should see an output file in this folder named something like <out.\w+>. This files contain the results. 

**CSV**  
The CSV file was made by hand using the outputs of the SLURM script described above. We do not currently have a scripted way to produce a CSV file from MergeSort output.

### QuickSort:  

**STATUS:**  Working!

**RUNNING QUICKSORT**  
After Cloning:
To run QuickSort, simply *cd* to the Mergesort file ```cd ./quickSort``` and submit your the job.slurm job file to the HPC enviroment by running ```sbatch job.slurm```. When the procedure is done, you should see an output file in this folder named something like <out.\w+>. This files contain the results.

**CSV**  
The CSV file was made by hand using the outputs of the SLURM script described above. We do not currently have a scripted way to produce a CSV file from QuickSort output.

### HeapSort:  

**STATUS:**  Working, nonparallizable 

**RUNNING HEAPSORT**  
After Cloning:
To run HeapSort, simply *cd* to the Mergesort file ```cd ./heapSort``` and submit your the job.slurm job file to the HPC enviroment by running ```sbatch job.slurm```. When the procedure is done, you should see an output file in this folder named something like <out.\w+>. This files contain the results.

### ShellSort:

**STATUS:**  Working with slight modifications, not yey implementable in OpenMP

**RUNNING SHELLSORT** 
*coming with next version of OMP.* However, you can find a working serial program of shellsort and the skeleton of a parallisable one in the ./shellSort folder.

## Analysis  

Please refer to the ***paper***

## FAQ's

Please email one of the authors in the title of ***paper***


