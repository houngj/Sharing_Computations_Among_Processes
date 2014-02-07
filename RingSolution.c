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
  double global_elapsed = 0;
  double local_start, local_finish, local_elapsed, elapsed, elapsed2;
  int t = 0;
  for(t = 0; t < 10; t++){
    MPI_Barrier(MPI_COMM_WORLD);
    unsigned int seed = my_rank;
    int randomnum = rand_r(&seed);
    local_start = MPI_Wtime();
    
    min = randomnum;
    max = min;
    usleep(rand_r(&seed) % 1000);
    //first phase
    
    
    if(my_rank == 0 && comm_sz != 1){
      message = min;
      MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
      message = max;
      MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
      MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      min = message;
      MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      max = message;
      //printf("--------rank-%02d-------- min - %d max - %d\n", my_rank, min, max);
    } else if(comm_sz != 1) {
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
    if(my_rank == 0 && comm_sz != 1){
      message = min;
      MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
      message = max;
      MPI_Send(&message, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
      MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&message, 1, MPI_INT, comm_sz-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if(comm_sz != 1){
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
    global_elased = elapsed+elapsed2+global_elapsed;
  }
  MPI_Finalize();
  
  if (my_rank == 0)
    printf("%f\n", global_elapsed/10.0);
  return 0;
}
