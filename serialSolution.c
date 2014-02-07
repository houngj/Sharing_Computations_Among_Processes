#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char * argv[]){
  int i = 0;
  srand(time(NULL));
  int min = -1;
  int max = -1;
  int r;
  struct timeval tv1, tv2;
  gettimeofday(&tv1, NULL);
  
  for(i = 0; i < atoi(argv[1]); i++){
   r = rand();
   usleep(r%1000);
   if(min == -1){
     min = r;
   }
   if(max == -1){
     max = r;
   }
   if(min > r){
     min = r;
   }
   if(max < r){
     max = r;
   }
  }
  gettimeofday(&tv2, NULL);
  printf("%f\n", (double)(tv2.tv_usec - tv1.tv_usec) /1000000 + (double)(tv2.tv_sec - tv1.tv_sec));


}
