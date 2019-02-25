#include <stddef.h>

int strcmp(char *str1, char *str2)
{
  char *s1 = str1;
  char *s2 = str2;

  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

int strncmp(char *str1, char *str2, size_t n)
{
  char *s1 = str1;
  char *s2 = str2;
  
  if (n == 0)
    return 0;

  while (n-- != 0 && *s1 == *s2) {
    if (n == 0 || *s1 == '\0')
      break;
    s1++;
    s2++;
  }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

char *strchr(char *s1, int i)
{
  unsigned char *s = s1;
  unsigned char c = i;

  while (*s && *s != c)
    s++;
  if (*s == c)
    return (char *)s;

    return NULL;
}

char *strrchr(char *s, int i)
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

size_t strlen(char *s)
{
  char *start = (char *)s;
  char *cur = start;
  
  while (*cur)
    cur++;
  
  return cur - start;
}

