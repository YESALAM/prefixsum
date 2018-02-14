#include <stdio.h>
#include <stdlib.h>

#define size 100000000

int main(){
	int* list;
	list = (int*) malloc(sizeof(int)*size);
	for(int i=0;i<size;i++) list[i] = i+1 ;
	
	int* result = malloc(sizeof(int)*size);
	result[0] = list[0];
	for(int i=1;i<size;i++) result[i] = result[i-1]+list[i];

	printf("Last item is %d\n ",result[size-1]);
	return 0 ;
}


