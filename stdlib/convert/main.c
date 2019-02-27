#include <stdio.h>
#include "convert.h"

int test_char2hex(void) {
  int res = 1;
  
  res = res && (char2hex('3')== 3);
  res = res && (char2hex('b')==11);
  res = res && (char2hex('D')==13);
  res = res && (char2hex('*')==-1);
  res = res && (char2hex('g')==-1);
  
  if(res == 0)
    printf("char2hex ok");
  else
    printf("char2hex error");
    
  return res;
}


void test()
{
    test_char2hex();

}
int main()
{
    test();
}
