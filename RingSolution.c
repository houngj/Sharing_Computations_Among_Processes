#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
const int MAX_STRING = 100;

int main(int argc, char *argv[]){
  unsigned int seed = 10;
  char message[2];
  int comm_sz;
  int my_rank;
  int min;
  int max;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  int A[comm_sz];
  
  int i = 0;
  for(i = 0; i < comm_sz; i++){
    A[i] = rand_r(&seed);
  }

  



}
