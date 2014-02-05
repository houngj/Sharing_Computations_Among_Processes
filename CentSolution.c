#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int main(int argc, char * argv[]){
  
  int message;
  int comm_sz; //number of processes
  int my_rank; //my process rank
  int min;
  int max;
  
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  unsigned int seed = my_rank;
  double local_start, local_finish, local_elapsed, elapsed;
  MPI_Barrier(MPI_COMM_WORLD);
  local_start = MPI_Wtime();
  if(my_rank != 0){
    message = rand_r(&seed);
    usleep(rand_r(&seed)%1000);
    MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    min = message;
    MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    max = message;
    //printf("----process-%02d----min - %d max - %d\n",my_rank, min, max);
  }
  else {
    
    min = rand_r(&seed);
    max = rand_r(&seed);
    usleep(rand_r(&seed)%1000);
    
    int q = 1;
    for(q = 1; q < comm_sz; q++){
      MPI_Recv(&message, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int current = message;
      if (current < min){
	min = current;
      } 
      if (current > max){
	max = current;
      }
    }
    //printf("----process-%02d---- min - %d max - %d\n", my_rank, min, max);
    for(q = 1; q < comm_sz; q++){
      message = min;
      MPI_Send(&message, 1, MPI_INT, q, 0, MPI_COMM_WORLD);
      message = max;
      MPI_Send(&message, 1, MPI_INT, q, 0, MPI_COMM_WORLD);
    }  
  }
  local_finish = MPI_Wtime();
  local_elapsed = local_finish - local_start;
  MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  if (my_rank == 0)
    printf("%f\n", elapsed);
  
  return 0;
}
