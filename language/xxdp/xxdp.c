#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void xxdp_encode(void *addr, uint32_t size)
{
    int index;

    while (size > 0) {
        index = 0;
        while (index < 30) {
            fprintf(stdout, "%02x", *((unsigned char *)addr));
            
            index++;
            addr++;
            size--;
 
            if (size <= 0)
                break;
        }
        fprintf(stdout,"\n");
    }
    return;
}
