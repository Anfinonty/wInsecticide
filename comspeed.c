
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  struct timeval time_start,time_now;
  int tick,i,j,c=2048,d=5;
  double sum=0,sum2=0;
  for (j=0;j<d;j++) {
    for (i=0;i<c;i++) {
      mingw_gettimeofday(&time_start, NULL);
      for (tick=0;tick<65536;tick++);
      mingw_gettimeofday(&time_now, NULL);
      sum += (time_now.tv_sec - time_start.tv_sec) * 1000000 + time_now.tv_usec - time_start.tv_usec;
      //printf("took %lu us\n", (time_now.tv_sec - time_start.tv_sec) * 1000000 + time_now.tv_usec - time_start.tv_usec);
    }
    printf("\nFinal: %3.5f\n",sum/c);
    sum2+=sum/c;
    sum=0;
  }
  printf("\nLast Final: %3.5f\n",sum2/d);
  return 0;
}
