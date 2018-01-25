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