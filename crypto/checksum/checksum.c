#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    unsigned char checksum = 0;
	unsigned char tmp = 0;
    if(argc < 2){
        printf("%s 0xAA 0xBB....\n", argv[0]);
	}

    for(int i=1; i<argc;i++){
        tmp= (unsigned char)strtoul(argv[i], NULL, 16);
		//printf("tmp is 0x%02x\n", tmp);
		checksum ^= tmp;
	}
    printf("0x%02x", checksum);
}