#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
const int MAX_STRING = 100;
int main(int argc, char * argv[]){
  
  char message[2];
  int comm_sz;
  int my_rank;
  int min;
  int max;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  unsigned int seed = my_rank;
  int randnum = rand_r(&seed); 
  
  min = randnum%100;
  printf("%d randomnum\n", randnum % 100);
  max = min;
  sprintf(message, "%d", min);
  sleep(randnum%10);
  int i = 0;
  for(i = 0; i < comm_sz; i++){
    if(i != my_rank){
      
      MPI_Send(message, strlen(message)+1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  }
  
  for(i = 0; i < comm_sz; i++){
    if(i != my_rank){
      MPI_Recv(message, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int current = atoi(message);
      if(current < min){
	min = current;
      }
      if(current > max){
	max = current;
      }
    }
  }
  
  printf("--------%d--------\nmin - %d\nmax - %d\n", my_rank, min, max);
  MPI_Finalize();
  return 0;

  
}
