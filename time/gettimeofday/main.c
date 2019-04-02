#include <stdio.h>
#include <unistd.h>

#include <inttypes.h>
#include <sys/time.h>

#define LOOP_TIME 5

void profiler(void (*func)(void))
{
  int loop = LOOP_TIME;

  struct timeval start;
  struct timeval end;

  gettimeofday(&start,NULL);
  for(int i=0; i<loop; i++) {
    func();
  }
  gettimeofday(&end,NULL);

  uint64_t time_us = (end.tv_sec-start.tv_sec)*1000*1000 + (end.tv_usec-start.tv_usec);
  printf("time is %"PRIu64" ms\n", time_us / 1000);
}

void func(void)
{
    sleep(1);
}


int main(void)
{
    profiler(func);
}
