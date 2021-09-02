#include <stdio.h>
#include <string.h>
#include "bootargs.h"

#define VALUESIZE (1024)
int main()
{
	char value[VALUESIZE] = {0};
	memset(value, 0, VALUESIZE);
	int len;
    bootargs_parse();
	
	char *param = "rootfstype=";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

	param = "root=";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

	param = "init=";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

	param = "ubi.mtd=";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

	param = "console=";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

	param = "rootwait";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

	param = "xxxxota";
	len = bootargs_get(param, value, VALUESIZE);
	printf("param:%s %s, len %d\n", param, value, len);
	memset(value, 0, VALUESIZE);

}
