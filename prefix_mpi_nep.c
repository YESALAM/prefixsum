#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<math.h>
#include<omp.h>

//#define LIST_SIZE  8

int main(int argc,char *argv[]){
	int world_rank;
	int world_size;
	int status = MPI_Init(&argc,&argv);
	if(status!=MPI_SUCCESS){
		printf("MPI initialization Problem");
		return 0 ;
	}
	
	int * list ;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	int LIST_SIZE = world_size;
	if(world_rank==0){
			list = (int *) malloc(LIST_SIZE*sizeof(int));
			for(int i=0;i<LIST_SIZE;i++){
				list[i] = i+1;
			}
	}
	int *sub_list = malloc(sizeof(int)*1);
	int *prefix = malloc(sizeof(int)*1);
	MPI_Scatter(list,1,MPI_INT,sub_list,1,MPI_INT,0,MPI_COMM_WORLD);
	
	MPI_Scan(sub_list,prefix,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
		
	int *prefix_list;
	if(world_rank==0){
		prefix_list = malloc(sizeof(int)*world_size);
	}
	
	MPI_Gather(prefix,1,MPI_INT,prefix_list,1,MPI_INT,0,MPI_COMM_WORLD);
	

	if(world_rank==0){
		for(int i=0;i<LIST_SIZE;i++) printf("%d ",prefix_list[i]);
	}

	MPI_Finalize();
	return 0;	
}
