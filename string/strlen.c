#include "string.h"

size_t strlen(const char *s)
{
  char *start = (char *)s;
  char *cur = start;
  
  while (*cur)
    cur++;
  
  return cur - start;
}
