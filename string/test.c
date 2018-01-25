#include <stdio.h>
#include "string.h"

int main()
{
  char *str = "This is a string";
  printf("strlen is %d\n", strlen(str));


  int ret = strcmp("hello", "world");
  printf("strcmp ret is %d\n", ret);
  ret = strcmp("hello", "hello");
  printf("strcmp ret is %d\n", ret);
}
