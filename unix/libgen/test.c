//gcc -o main test_libgen.c ../unix/basename.c ../unix/dirname.c -I../include
#include <stdio.h>

#include "libgen.h"

int main()
{
	char path[] = "/root/gnu/linux/kernel.img";
	char *p;
	
	p = dirname(path);
	printf("dirname is %s\n", p);

	p = basename(path);
	printf("basename is %s\n", p);
}
