#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size 1000000

int main(){
	clock_t start,finish ;
	start = clock();
	int* list;
	list = (int*) malloc(sizeof(int)*size);
	for(int i=0;i<size;i++) list[i] = i+1 ;
	
	int* result = malloc(sizeof(int)*size);
	result[0] = list[0];
	for(int i=1;i<size;i++) result[i] = result[i-1]+list[i];

	printf("Last item is %d\n ",result[size-1]);
	finish = clock();
	printf("Time (seconds): %0.3lf\n", ((double)(finish - start))/CLOCKS_PER_SEC);
	return 0 ;
}


