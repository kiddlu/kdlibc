#ifndef _CONVERT_H_
#define _CONVERT_H_
int char2hex(char c);
char hex2char(int h);
char *utoa(unsigned int value, char *str, int base);
char* itoa(int value, char* result, int base);
int atoi(const char *str);
#endif
