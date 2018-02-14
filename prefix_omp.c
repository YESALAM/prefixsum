#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define THREADS 2
#define size  1000000

int main(int argc, char *argv[]) {
	clock_t start, finish;
 	start = clock();

	int maxThreads = THREADS ;//omp_get_max_threads();
	double* elapsed ;
	elapsed = (double*)malloc(sizeof(double)*maxThreads);

	int *list ;
	int outer = ceil(log(size)/log(2));
	list = (int*)malloc(sizeof(int)*size);
	for(int i=0;i<size;i++){
		list[i] = i+1 ;
	}
	int *temp = (int*)malloc(sizeof(int)*size);
	int* current ;
	int* prev ;
	for (int i=0;i<1;i++){		
		if(i%2==0) {
			current = temp ;
			prev  = list ;
		}else{
			current = list;
			prev = temp;
		}	
		#pragma omp parallel for schedule(static) num_threads(THREADS)
		for(int j=0;j<size;j++){	
			int th;
			double t1, t2;

			th = omp_get_thread_num();
			//printf("Thread %d executes job #%d (sleep %g sec).\n", th, i, jobs[i] / 1e6);
			t1 = omp_get_wtime();
			//usleep(jobs[i]);
			
			int poi = pow(2,i);		
			if(j<poi) 
				current[j] = prev[j] ;
			else
				current[j] = prev[j] + prev[j-poi];
			if((j%250000)==0) printf("Thread no %d\n",omp_get_thread_num());
			t2 = omp_get_wtime();
			elapsed[th] += t2 - t1;
		}
	}

	double total = 0 ;
	for (int i = 0; i < maxThreads; ++i) {
        printf("Elapsed time for thread %d: %g\n", i, elapsed[i]);
		total += elapsed[i];
    }
	printf("Total elapsed time %g\n",total);

	//for(int i=0;i<size;i++)
	//	printf("%d ",a[outer][i]);
	printf("%d\n",current[size-1]);
	printf("\n");
	finish = clock();
	printf("Time (seconds): %0.3lf\n",((double)(finish-start))/CLOCKS_PER_SEC);
}
