#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <omp.h>

void swap(int *array, int i, int j) { 
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
}

//gapped insertion sort
void insertionSort(int *array, int array_length, int gap_size) {
	int current_position;
	for(current_position = gap_size; current_position < array_length; current_position += gap_size){
		int last_positions;
		last_positions = current_position - gap_size;
		//USE BLOCK SIZES#pragma omp parallel for shared(array, array_length, gap_size) 
		for( ; (last_positions > -1) && (array[last_positions + gap_size] <= array[last_positions]);  ){
			swap(array, last_positions + gap_size, last_positions);
			last_positions = last_positions - gap_size;			
		}
	}
}

//
void shellSort(int *array, int array_length){
	int i, gap_sizes;
    	for(gap_sizes = array_length / 2; gap_sizes > 0; gap_sizes /= 2){	
    		//#pragma omp parallel for shared(array, gap_sizes, array_length) private (i) default(none)
		for(i = 0; i < array_length; i++)
              		insertionSort(array, array_length, gap_sizes);
	}
}




void arrayPrint(int *array, int print_length) {
    int i;
    for (i = 0; i < print_length; i++) {
	if (i == print_length-1){
        	printf("%d)\n", array[i]);
	}
	else if(i == 0){
		printf("(%d, ", array[i]);
	}
	else {
		printf("%d, ", array[i]);
	}
    }
    printf("\n");
}

uint32_t rand4byte(){
	return (uint32_t) (rand() % (2^16 - 1));
}


//warning this only works on systems where int size is 4 bytes.
uint32_t randomUInt32(){ 
   return (((uint32_t) rand4byte() << 16) ^ ((unsigned int) rand4byte()));  
}


void shuffle(int *array, int array_length, int seed){
   	int i;
	int pass;
	for (pass = 0; pass < seed; pass++){
		for (i = 0; i < array_length; i++) {
        		int position1 = randomUInt32() % array_length;
        		int position2 = randomUInt32() % array_length;
        		swap(array, position1, position2);
    		}
        }
	
}

void arrayGen(int array_to_be[], int array_length){
        int i;
        for(i = 0; i < array_length; i++){
                array_to_be[i] = i + 1;
        }
}


// shuffle seed is num passes of randomiser.
int main(int argc, char *argv[]){
	int shuffle_seed;
	int array_length;
	shuffle_seed = atoi(argv[1]);
	array_length = atoi(argv[2]);
	printf("seed: %d\n", shuffle_seed);
	printf("length: %d\n", array_length);
	int input[array_length];
	arrayGen(input, array_length);
	arrayPrint(input, array_length);	
	
	shuffle(input, array_length, shuffle_seed);
	
	arrayPrint(input, array_length);	
	time_t start_time, end_time;
	double duration;

	//start_time = omp_get_wtime();
	//startsort
	insertionSort(input, array_length, 1);
	//shellSort(input, array_length);	
	//endsort
	//#pragma barrier
	//end_time = omp_get_wtime();
	//duration = difftime(end_time, start_time);
	printf("%f\n", duration);
	arrayPrint(input, array_length);
	return 0;
}
