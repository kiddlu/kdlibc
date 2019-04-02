#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define LOOP_TIME 5

void profiler(void (*func)(void))
{
  int loop = LOOP_TIME;

  time_t begin;
  time_t end ;

  begin = clock();
  for(int i=0; i<loop; i++) {
    func();
  }
  end = clock();
  printf("difftime is %.2f \n\n", difftime(end,begin));
}

void func(void)
{
    sleep(1);
}


int main(void)
{
    profiler(func);
}
