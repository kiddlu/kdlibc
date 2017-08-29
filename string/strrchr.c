#include "string.h"

char *strrchr(const char *s1, int i)
{
  const char *last = NULL;
  
  if (i) {
    while ((s=strchr(s, i))) {
        last = s;
        s++;
    }
  } else {
    last = strchr(s, i);
  }
            
  return (char *) last;
}