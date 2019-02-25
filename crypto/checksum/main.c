#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

unsigned char checksum (unsigned char *ptr, size_t sz) {
  unsigned char chk = 0;
  while (sz-- != 0)
    chk -= *ptr++;
  return chk;
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
	printf("Checksum of file %s (%d bytes) is 0x%02X\n", argv[1], len, checksum(data,len));
  return 0;
}
