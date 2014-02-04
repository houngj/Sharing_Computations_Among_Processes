#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

const int MAX_STRING = 100;
int main(int argc, char * argv[]){
  
  
  char message[2];
  int comm_sz; //number of processes
  int my_rank; //my process rank
  int min = 100;
  int max = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  unsigned int seed = my_rank;
  /*int A[comm_sz];
  int i = 0;
  for (i = 0; i < comm_sz; i++){
    A[i] = rand_r(&seed);
    
    }*/

  if(my_rank != 0){
    sprintf(message, "%d", rand_r(&seed) % 100);
    
    sleep(rand_r(&seed) % 10);
    
    MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(message, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = atoi(message);
    MPI_Recv(message, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = atoi(message);
    printf("----process-%d seed-%d----\nmin - %d\nmax - %d\n",my_rank, seed, min, max);
  }
  else {
    //printf("%d\n", A[my_rank] % 100);
    min = rand_r(&seed) % 100;
    max = rand_r(&seed) % 100;
    
    sleep(rand_r(&seed) % 10);
    int q = 1;
    for(q = 1; q < comm_sz; q++){
      MPI_Recv(message, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int current = atoi(message);
      if (current < min){
	min = current;
      } 
      if (current > max){
	max = current;
      }
      
    }
    printf("----process-%d seed-%d----\nmin - %d\nmax - %d\n",my_rank, seed, min, max);
    for(q = 1; q < comm_sz; q++){
      sprintf(message, "%d", min);
      MPI_Send(message, strlen(message)+1, MPI_CHAR, q, 0, MPI_COMM_WORLD);
      sprintf(message, "%d", max);
      MPI_Send(message, strlen(message)+1, MPI_CHAR, q, 0, MPI_COMM_WORLD);
    }
    
  }


  MPI_Finalize();

  return 0;
}
