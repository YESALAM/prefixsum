#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<math.h>

#define LIST_SIZE  8000000

int main(int argc,char *argv[]){
	int rank;
	int size;
	int status = MPI_Init(&argc,$argv);
	if(status!=MPI_SUCCESS){
		printf("MPI initialization Problem");
		return 0 ;
	}
	
	MPI_Comm_rank(&rank);
	MPI_Comm_size(&size);
	
	int * list = NULL ;
	int *temp = NULL ;
	if(rank == 0 ){
		list = (int *) malloc(sizeof(int) * LIST_SIZE );
		int chunk_size = LIST_SIZE  / size ;
		for(int i=1;i<size;i++){
			int *sub_list = list+chunk_size ;
			MPI_Send(list,chunk_size,MPI_INT,i,0,MPI_COMM_WORLD);			
		}
		int log_of_chunk_size  = ceil(log(size)/log(2));
		for(int i=0;i<log_of_chunk_size;i++){
			
			for(int j=0;j<chunk_size;j++){
				int poi = pow(2,i);
				temp = (int *)malloc(sizeof(int) * chunk_size*2);
				int f_index;
				if(i%2==0) f_index = 1;
				else f_index = 0;
				if(j<poi){
					temp[f_index][j] = temp[f_index^1][j];	
				}else{
					temp[f_index][j] = temp[f_index^1][j] + temp[j-poi];
				}
			}
		}
		int *last_item = &temp[0][chunk_size-1];
		MPI_Send(last_item,1,MPI_INT,1,1,MPI_COMM_WORLD);

	}
}
