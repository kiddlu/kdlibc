void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *src = (const unsigned char *) s;
    unsigned char d = c;

    while (n--) {
        if (*src == d)
            return (void *) src;
        else
            src++;
    }
    return NULL;
}

int memcmp(const void *m1, const void *m2, size_t n)
{
    unsigned char *s1 = (unsigned char *) m1;
    unsigned char *s2 = (unsigned char *) m2;

    while (n--) {
        if (*s1 != *s2) {
	        return *s1 - *s2;
	} else {
            s1++;
            s2++;
        }
    }
    return 0;
}

void *memcpy(void *dst, const void *src, size_t n)
{
  char *_dst = (char *) dst;
  char *_src = (char *) src;

  void *save = dst;

  while (n--) {
      *_dst++ = *_src++;
  }

  return save;
}

void *memset(void *m, int c, size_t n)
{
    char *s = (char *) m;
    while (n--)
        *s++ = (char) c;

  return m;
}
