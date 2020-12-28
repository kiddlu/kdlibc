#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


char mem_buf[]={0x93,0x18,0xe4,0x3f};

void memdump(void *addr, uint32_t size)
{
    int index;
    
    while (size > 0) {
        fprintf(stdout, "%p: ", addr);
        index = 0;

        // 32 bytes in the line
        while (index < 32) {
            fprintf(stdout, "0x%02x ", *((unsigned char *)addr));
            
            index++;
            addr++;
            size--;
 
            if (size <= 0)
                break;
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
    return;
}

int main()
{
	//float *p = (float *)mem_buf;
	//float v = *p;
	//printf("mem2float is %f\n", v);
	printf("mem2float is %f\n", *((float *)mem_buf));
	
	float value = 1.782;
	memdump(&value, sizeof(value));
}
