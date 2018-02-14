#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define size  100000000

int main(int argc, char *argv[]) {
	int *list ;
	int outer = ceil(log(size)/log(2));
	list = (int*)malloc(sizeof(int)*size);
	for(int i=0;i<size;i++){
		list[i] = i+1 ;
	}
	int *temp = (int*)malloc(sizeof(int)*size);
	int* current ;
	int* prev ;
	for (int i=0;i<outer;i++){		
		if(i%2==0) {
			current = temp ;
			prev  = list ;
		}else{
			current = list;
			prev = temp;
		}	
		#pragma omp parallel for 
		for(int j=0;j<size;j++){	
			int poi = pow(2,i);		
			if(j<poi) 
				current[j] = prev[j] ;
			else
				current[j] = prev[j] + prev[j-poi];
			//printf("Thread no %d\n",omp_get_thread_num());
		}
	}
	//for(int i=0;i<size;i++)
	//	printf("%d ",a[outer][i]);
	printf("%d\n",current[size-1]);
	printf("\n");
	
}
