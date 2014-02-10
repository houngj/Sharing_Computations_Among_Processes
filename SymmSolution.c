#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
const int MAX_STRING = 100;
int main(int argc, char * argv[]){
  
  int message;
  int comm_sz;
  int my_rank;
  int min;
  int max;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  FILE* output = stdin;
  if(argv[1] != NULL){
    output = stdout;
  }
  double local_start, local_finish, local_elapsed, elapsed;
  
  
  unsigned int seed = my_rank;
  MPI_Barrier(MPI_COMM_WORLD);
  int randnum = rand_r(&seed); 
  local_start = MPI_Wtime();
  min = randnum;
  
  max = min;
  message = min;
  usleep(randnum%1000);
  int i = 0;
  for(i = 0; i < comm_sz; i++){
    if(i != my_rank){
      MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  }
  for(i = 0; i < comm_sz; i++){
    if(i != my_rank){
      MPI_Recv(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int current = message;
      if(current < min){
	min = current;
      }
      if(current > max){
	max = current;
      }
    }
  }
  
  fprintf(output, "--------rank-%02d-------- min - %d max - %d\n", my_rank, min, max);
  local_finish = MPI_Wtime();
  local_elapsed = local_finish - local_start;
  MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  

  MPI_Finalize();
  if (my_rank == 0)
    printf("%f\n", elapsed);
  return 0;

  
}
