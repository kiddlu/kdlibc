char *strchr(const char *s1, int i)
{
  unsigned char *s = s1;
  unsigned char c = i;

  while (*s && *s != c)
    s++;
  if (*s == c)
    return (char *)s;

    return NULL;
}