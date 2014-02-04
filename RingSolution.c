#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
const int MAX_STRING = 100;

int main(int argc, char *argv[]){
  
  char message[2];
  int comm_sz;
  int my_rank;
  int min;
  int max;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  unsigned int seed = my_rank;
  
  int randomnum = rand_r(&seed) % 100;
  min = randomnum;
  max = min;
  sleep(rand_r(&seed) % 10);
  //first phase
  
  
  if(my_rank == 0){
    sprintf(message, "%d", min);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank+1, 0, MPI_COMM_WORLD);
    sprintf(message, "%d", max);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(message, MAX_STRING, MPI_CHAR, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = atoi(message);
    MPI_Recv(message, MAX_STRING, MPI_CHAR, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = atoi(message);
    printf("--------rank-%02d-------- min - %d max - %d\n", my_rank, min, max);
  } else {
    MPI_Recv(message, MAX_STRING, MPI_CHAR, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = atoi(message);
    MPI_Recv(message, MAX_STRING, MPI_CHAR, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = atoi(message);
    if(randomnum < min){
      sprintf(message, "%d", randomnum);
    }
    else{
      sprintf(message, "%d", min);
    }
    MPI_Send(message, strlen(message)+1, MPI_CHAR, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
    if(randomnum > max){
      sprintf(message, "%d", randomnum);
    }
    else {
      sprintf(message, "%d", max);
    }
    MPI_Send(message, strlen(message)+1, MPI_CHAR, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
    
    
  }

  //second phase
  
  if(my_rank == 0){
    sprintf(message, "%d", min);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank+1, 0, MPI_COMM_WORLD);
    sprintf(message, "%d", max);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank+1, 0, MPI_COMM_WORLD);
  }
  else{
    MPI_Recv(message, MAX_STRING, MPI_CHAR, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = atoi(message);
    MPI_Recv(message, MAX_STRING, MPI_CHAR, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = atoi(message);
    printf("--------rank-%02d-------- min - %d max - %d\n", my_rank, min, max);
    sprintf(message, "%d", min);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
    sprintf(message, "%d", max);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
  return 0;
}
