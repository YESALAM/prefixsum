#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define size  1000000
int a[21][size];

int main(int argc, char *argv[]) {
	clock_t current ;
	current = clock(); 	
	
	int outer = ceil(log(size)/log(2));
	//int a[outer+1][size] ;
	//int input[] = {1,2,3,4,5,6} ;
	for(int i=0;i<size;i++){
		a[0][i] = i+1 ;
	}
	for (int i=0;i<outer;i++)
		for(int j=0;j<size;j++){	
			int poi = pow(2,i);		
			if(j<poi) 
				a[i+1][j] = a[i][j] ;
			else
				a[i+1][j] = a[i][j] + a[i][j-poi];
		}
	
	/*for(int i=0;i<size;i++)
		printf("%d ",a[outer][i]);*/
	printf("\n");
	current = clock()-current;
	double time_elapsed = ((double)current)/CLOCKS_PER_SEC ;
	printf("Time taken %f\n",time_elapsed);
}
