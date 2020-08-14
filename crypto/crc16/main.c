
/* 
    This polynomial ( 0xEDB88320L) DOES generate the same CRC values as ZMODEM and PKZIP
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
 
uint16_t crc16(const unsigned char *buf, uint32_t size)
{

	int i, j = 0;
	int crc16 = 0xffff;

	for (i = 0; i < size; i++) {
		crc16 = crc16 ^ buf[i];
		for (j = 0; j < 8; j++) {
			if (crc16 & 0x01)
				crc16 = (crc16 >> 1) ^ 0xa001;
			else crc16 = crc16 >> 1;
		}
	}
	return crc16;
}

unsigned int get_file_size(FILE *fp)
{
    unsigned int length;

    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    return length;
}

void usage(void)
{
	printf("useage: file\n");
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        usage();
        return -1;
    } else if ( 0 != access(argv[1], F_OK)) {
		usage();
		return -1;
	}

	FILE *fp = fopen(argv[1], "rb");
    uint32_t len = get_file_size(fp);

    char *data = malloc(len);
    fread(data, len, 1, fp);
	
	printf("\n");
	printf("CRC16 of file %s (%d bytes) is 0x%04X\n", argv[1], len, crc16(data,len));
    return 0;
}
