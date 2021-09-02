#ifndef _BOOTARGS_H_
#define _BOOTARGS_H_

//if param has value return value len
//if param has no value return 0
//if param null / not found return -1
int bootargs_get(const char* param, char *value, int max_len);
int bootargs_parse(void);

#endif