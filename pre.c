#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define size  1000000
int a[21][size];

int main(int argc, char *argv[]) {
	struct timeval start,end ;
	gettimeofday(&start,NULL);
  	int iam = 0, np = 6;
	
	int outer = ceil(log(size)/log(2));
	//int a[outer+1][size] ;
	//int input[] = {1,2,3,4,5,6} ;
	for(int i=0;i<size;i++){
		a[0][i] = i+1 ;
	}
	for (int i=0;i<outer;i++)
		
		#pragma omp parallel for 
		for(int j=0;j<size;j++){	
			int poi = pow(2,i);		
			if(j<poi) 
				a[i+1][j] = a[i][j] ;
			else
				a[i+1][j] = a[i][j] + a[i][j-poi];
			//printf("Thread no %d\n",omp_get_thread_num());
		}
	
	//for(int i=0;i<size;i++)
	//	printf("%d ",a[outer][i]);
	printf("\n");
	
	gettimeofday(&end,NULL);
	double time_elapsed = (double)(end.tv_usec-start.tv_usec)/ 1000 ;
	printf("Time taken %f\n",time_elapsed);
}
