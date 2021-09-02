#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BOOTARGS_SIZE   (1024)

static char bootargs_buf[BOOTARGS_SIZE];

static int already_parse = 0;

//if param has value return value len
//if param has no value return 0
//if param null / not found return -1
int bootargs_get(const char* param, char *value, int max_len)
{
    int param_len = strlen(param);
	int value_len = 0;
	int ret = 0;
	char *argsp, *stop;

	if(param_len == 0)
	{
		printf("param null?\n");
        return -1;
	}

	//printf("%s, %d\n", param, param_len);
    argsp = strstr(bootargs_buf, param);
	//printf("argsp %p; bootargs_buf %p\n", argsp, bootargs_buf);
    if(argsp) {
		argsp += param_len;
        if(argsp[-1] == '='){
            //printf("param has value\n");
			stop = argsp;
			while(*stop != ' ' && *stop != '\n'){
				stop++;
				value_len++;
			}
			//printf("value_len %d\n", value_len);
			if(value_len > max_len) {
				ret = max_len;
			} else {
				ret = value_len;				
			}
			memcpy(value, argsp, ret);
        } else if(argsp[0] == ' ') {
            //printf("param no value\n");
			ret = 0;			
        } else {
            //printf("param not complete\n");
			ret = -1;
        }
        return ret;
    } else {
        //printf("param not found\n");
        return -1;
    }
}


int bootargs_parse()
{
	int fd;
	ssize_t n;

    if(already_parse){
        return 0;
    }

	//fd = open("/proc/cmdline", O_RDONLY);
	fd = open("./cmdline", O_RDONLY);
	if(fd >= 0) {
		n = read(fd, bootargs_buf, sizeof(bootargs_buf)-1);
		if(n > 0) {
			bootargs_buf[n] = 0;
		}
		close(fd);
        already_parse = 1;
	} else {
        return -1;
    }

	return 0;
}
