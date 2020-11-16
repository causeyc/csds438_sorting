#include<stdio.h>
#include<stdlib.h> 

void arrayPrint(int array[], int print_length) {
    int i;
    for (i = 0; i < print_length; i++) {
        if (i == print_length - 1){
                printf("%d)", array[i]);
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

void arrayGen(int array_to_be[], int array_length){
        int i;
        for(i = 0; i < array_length; i++){
                array_to_be[i] = i;
		printf("%d\n", i);
        }
}


int main(int argc, char *argv[]){
	int len;
	len = atoi(argv[1]);
	printf("the length is now: %d\n", 1 / 2);
//	int test[sizeof(int) * length]; 
//	printf("the length is now: %d\n", length);
//	arrayGen(test, 5);
//	printf("the length is now: %d\n", length);
	//printf("\n \n %d\n", *(test+ 2));
//	arrayPrint(test, length);
//	arrayPrint(test, 5);
	return 0;
}
