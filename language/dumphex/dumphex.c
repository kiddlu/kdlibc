#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#if 1
#define dbg_printf printf
#else 
#define dbg_printf {;}
#endif

void dumphex_encode(void *data, uint32_t size)
{
	char ascii[17];
	unsigned int i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		dbg_printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			dbg_printf(" ");
			if ((i+1) % 16 == 0) {
				dbg_printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					dbg_printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					dbg_printf("   ");
				}
				dbg_printf("|  %s \n", ascii);
			}
		}
	}
}