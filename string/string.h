#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);

/*
char 	*_EXFUN(strcat,(char *__restrict, const char *__restrict));
char 	*_EXFUN(strchr,(const char *, int));
int	 _EXFUN(strcmp,(const char *, const char *));
int	 _EXFUN(strcoll,(const char *, const char *));
char 	*_EXFUN(strcpy,(char *__restrict, const char *__restrict));
size_t	 _EXFUN(strcspn,(const char *, const char *));
char 	*_EXFUN(strerror,(int));
char 	*_EXFUN(strncat,(char *__restrict, const char *__restrict, size_t));
int	 _EXFUN(strncmp,(const char *, const char *, size_t));
char 	*_EXFUN(strncpy,(char *__restrict, const char *__restrict, size_t));
char 	*_EXFUN(strpbrk,(const char *, const char *));
char 	*_EXFUN(strrchr,(const char *, int));
size_t	 _EXFUN(strspn,(const char *, const char *));
char 	*_EXFUN(strstr,(const char *, const char *));
char 	*_EXFUN(strtok,(char *__restrict, const char *__restrict));
*/
#endif