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