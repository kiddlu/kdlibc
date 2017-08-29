int strncmp(const char *str1, const char *str2, size_t n)
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