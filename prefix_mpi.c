#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<math.h>
#include<omp.h>

#define LIST_SIZE  1000000

int* prefix_sum_list(int *list,int size){
	int log_of_list_size  = ceil(log(size)/log(2));	
	int *temp = (int *)malloc(sizeof(int) * size);	
	int* current ;
	int* prev ;
	for(int i=0;i<log_of_list_size;i++){			
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
			if(j<poi){
				current[j] = prev[j];
			}else{
				current[j] = prev[j]+prev[j-poi];
			}
		}
	}
	free(prev);	
	return current;
}

void add_last_toAll(int *list,int size,int last_item){
	#pragma omp parallel for
	for(int i=0;i<size;i++){
		list[i] = list[i]+last_item ;
	}
}


int main(int argc,char *argv[]){
	int world_rank;
	int world_size;
	int status = MPI_Init(&argc,&argv);
	if(status!=MPI_SUCCESS){
		printf("MPI initialization Problem");
		return 0 ;
	}
	
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);

	 // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);	

	int chunk_size = LIST_SIZE  / world_size ;

	int * list = NULL ;
	int *temp = NULL ;
	if(world_rank == 0 ){
		
    	//printf("Hello world from processor %s, world_rank %d out of %d processors\n",processor_name, world_rank, size);

		list = (int *) malloc(sizeof(int) * LIST_SIZE );
		for(int i=0;i<LIST_SIZE;i++){
			list[i] = i+1;
		}
		int pointer = chunk_size;
		for(int i=1;i<world_size;i++){
			int *sub_list = list+pointer ;
			if(i==world_size-1) MPI_Send(sub_list,chunk_size+(LIST_SIZE%world_size),MPI_INT,i,0,MPI_COMM_WORLD);
			else MPI_Send(sub_list,chunk_size,MPI_INT,i,0,MPI_COMM_WORLD);	
			pointer = pointer+chunk_size ;					
		}


		temp = prefix_sum_list(list,chunk_size);
		
		int *result = (int* ) malloc(sizeof(int)*LIST_SIZE);
		pointer = 0;
		for(int i=0;i<chunk_size;i++){
			result[i]=temp[i];
		}
				
		
		if(world_size>1){
			int last_item = temp[chunk_size-1];
			MPI_Send(&last_item,1,MPI_INT,1,1,MPI_COMM_WORLD);
			
			
			int final_sum ;
			MPI_Recv(&final_sum,1,MPI_INT,world_size-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			

			for(int i=1;i<world_size;i++){		
				pointer = pointer + chunk_size ;			
				if(i == world_size-1) chunk_size = chunk_size + (LIST_SIZE%world_size) ;
				int *temp = (int*) malloc(sizeof(int)*chunk_size);
				MPI_Recv(temp,chunk_size,MPI_INT,i,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				for(int j=0 ;j<chunk_size;j++){
					result[pointer+j] = temp[j];
				}
				free(temp);
			}
			
		}

		printf("Final sum = %d\n",result[LIST_SIZE-1]);		

	}else{
				
		//printf("Hello world from processor %s, world_rank %d"," out of %d processors\n",processor_name, world_rank, world_size);
		if(world_rank == world_size-1) chunk_size = chunk_size + (LIST_SIZE%world_size) ;
		temp = (int*) malloc(sizeof(int) * chunk_size);		
		MPI_Recv(temp,chunk_size,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);	
		
		temp = prefix_sum_list(temp,chunk_size);

		int pre_last_item ;
		MPI_Recv(&pre_last_item,1,MPI_INT,world_rank-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		int curr_last_item = temp[chunk_size-1];
		add_last_toAll(temp,chunk_size,pre_last_item);

		int last_item = temp[chunk_size-1];
		
		if(world_rank != world_size-1) MPI_Send(&last_item,1,MPI_INT,world_rank+1,1,MPI_COMM_WORLD);
		else MPI_Send(&last_item,1,MPI_INT,0,1,MPI_COMM_WORLD);	


		MPI_Send(temp,chunk_size,MPI_INT,0,2,MPI_COMM_WORLD);

	}
	free(list);
	free(temp);
	MPI_Finalize();
	return 0;	
}
