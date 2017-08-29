int strcmp(const char *str1, const char *str2)
{
  char *s1 = str1;
  char *s2 = str2;

  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}