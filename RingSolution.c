#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
const int MAX_STRING = 100;

int main(int argc, char *argv[]){
  
  int message;
  int comm_sz;
  int my_rank;
  int min;
  int max;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  unsigned int seed = my_rank;
  double local_start, local_finish, local_elapsed, elapsed, elapsed2;
  MPI_Barrier(MPI_COMM_WORLD);
  int randomnum = rand_r(&seed);
  local_start = MPI_Wtime();
  
  min = randomnum;
  max = min;
  usleep(rand_r(&seed) % 1000);
  //first phase
  
  
  if(my_rank == 0){
    message = min;
    MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
    message = max;
    MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = message;
    MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = message;
    //printf("--------rank-%02d-------- min - %d max - %d\n", my_rank, min, max);
  } else {
    MPI_Recv(&message, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = message;
    MPI_Recv(&message, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = message;
    if(randomnum < min){
      message = randomnum;
    }
    else{
      message = min;
    }
    MPI_Send(&message, 1, MPI_INT, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
    if(randomnum > max){
      message = randomnum;
    }
    else {
      message = max;
    }
    MPI_Send(&message, 1, MPI_INT, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
    
    
  }

  local_finish = MPI_Wtime();
  local_elapsed = local_finish - local_start;
  MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
  //second phase
  MPI_Barrier(MPI_COMM_WORLD);
  local_start = MPI_Wtime();
  if(my_rank == 0){
    message = min;
    MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
    message = max;
    MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else{
    MPI_Recv(&message, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = message;
    MPI_Recv(&message, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = message;
    //printf("--------rank-%02d-------- min - %d max - %d\n", my_rank, min, max);
    message = min;
    MPI_Send(&message, 1, MPI_INT, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
    message = max;
    MPI_Send(&message, 1, MPI_INT, (my_rank+1) % comm_sz, 0, MPI_COMM_WORLD);
  }
  local_finish = MPI_Wtime();
  local_elapsed = local_finish - local_start;
  MPI_Reduce(&local_elapsed, &elapsed2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  if (my_rank == 0)
    printf("%f\n", (elapsed+elapsed2)/2.0);
  return 0;
}
