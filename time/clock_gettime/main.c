#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

#define LOOP_TIME 5

void profiler(void (*func)(void))
{
  int loop = LOOP_TIME;

  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for(int i=0; i<loop; i++) {
    func();
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  uint64_t time_us = (end.tv_sec-start.tv_sec)*1000*1000 + (end.tv_nsec-start.tv_nsec)/ 1000;
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
