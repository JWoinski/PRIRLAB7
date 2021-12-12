#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <stdbool.h>

#define ZASZCZEPIONY 0
#define ZARAZONY 1
#define OZDROWIENIEC 2


int valueinarray(int val, int arr[]){
    int i;
    for(i = 0; i < sizeof(arr); i++){
        if(arr[i] == val) return 1;
    }
    return 0;
}


  
  int index_zarazeni = 0;
  int index_zaszczepieni =0;
  int index_ozdrowiency=0;
  int stan;
  

void szczepienie(int world_size,int zaszczepieni[]){
	//printf("Populacja wynosi %d osob\n", world_size);
	
	  	stan = ZASZCZEPIONY;
	  
	
		sleep(1);
		int r = rand() % ((world_size-1) + 1 - 3) + 3;
		
		if(valueinarray(r,zaszczepieni))
		{
			printf("obywatel nr %d zaszczepiony ponownie\n",r);
		}
		else
		{
			MPI_Send(&stan,1,MPI_INT,r,0,MPI_COMM_WORLD);
			printf("obywatel nr %d zaszczepiony \n",r);
			zaszczepieni[index_zaszczepieni] = r;
			index_zaszczepieni++;
		}
	
}
	
void zarazanie(int world_size,int zarazone_watki[], int zaszczepieni[]){
	
	  
	  stan = ZARAZONY;
	  
	
		sleep(1);
		int r = rand() % (world_size-1 + 1 - 3) + 3;
		
		if(valueinarray(r,zarazone_watki))
		{
			printf("obywatel nr %d zarazony ponownie\n",r);
		}
		else if(valueinarray(r,zaszczepieni)){
			printf("obywatel nr %d zdazyl sie zaszczepic\n",r);
		}
		else
		{
			MPI_Send(&stan,1,MPI_INT,r,0,MPI_COMM_WORLD);
			printf("obywatel nr %d zarazony \n",r);
			zarazone_watki[index_zarazeni] = r;
			index_zarazeni++;
		}
			
 
}

void zdrowienie(int world_size,int ozdrowiency[]){
	
		
	  stan = OZDROWIENIEC;
	  
	
		sleep(1);
		int r = rand() % (world_size-1 + 1 - 3) + 3;
		
		if(!(valueinarray(r,ozdrowiency)))
		{
			MPI_Send(&stan,1,MPI_INT,r,0,MPI_COMM_WORLD);
			printf("obywatel nr %d wyzdrowial \n",r);
			ozdrowiency[index_ozdrowiency] = r;
			index_ozdrowiency++;
		}
  
  }
  
void podsumowanie(int world_size, int zarazeni[], int ozdrowiency[]){
	int licznik=0;
	printf("\n\nPodsumowanie:\n");
	for(int i=0; i<=world_size; i++){
		if(valueinarray(i,zarazeni) && !(valueinarray(i,ozdrowiency))){
			printf("obywatel numer %d zmarl w wyniku choroby\n",i);
			licznik++;
		}
		}
		printf("W wyniku epidemii zginelo %d osob\n",licznik);
}

void run(int world_size,int zaszczepieni[],int zarazone_watki[],int ozdrowiency[]){

	for(int i=3 ; i<world_size;i++)
	{
		szczepienie(world_size,zaszczepieni);
		zarazanie(world_size,zarazone_watki,zaszczepieni);
		zdrowienie(world_size,ozdrowiency);
	}

	podsumowanie(world_size,zarazone_watki,ozdrowiency);
}

int main(int argc, char** argv) {
 const int PING_PONG_LIMIT = 10;
	srand(time(NULL));
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  //Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
   int zarazone_watki[world_size];
  int zaszczepieni[world_size/2];
  int ozdrowiency[world_size];
  
if (world_rank == 0){
	/*szczepienie(world_size,zaszczepieni);
	zarazanie(world_size,zarazone_watki,zaszczepieni);
	zdrowienie(world_size,ozdrowiency);
	podsumowanie(world_size,zarazone_watki,ozdrowiency);*/
	run(world_size,zaszczepieni,zarazone_watki,ozdrowiency);
}


  
 MPI_Finalize();

return 0;

}
